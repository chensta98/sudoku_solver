// Declarations and functions for project #4

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

int RecursionCount = 0;
int TotalCount = 0;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class board
    // Stores the entire Sudoku board
{
public:
    board(int);
    void clear();
    void initialize(ifstream& fin);
    void print();
    bool isBlank(int, int);
    ValueType getCell(int, int);
    void setCell(int, int, int);
    void setCellConflicts(int, int);
    void setConflicts();
    void printConflicts();
    void findBlank();
    bool sudokuComplete();

private:

    // The following matrices go from 1 to BoardSize in each
    // dimension, i.e., they are each (BoardSize+1) * (BoardSize+1)

    matrix<ValueType> value;
    matrix<vector<int>> conflicts;
};

board::board(int sqSize)
    : value(BoardSize + 1, BoardSize + 1), conflicts(BoardSize + 1, BoardSize + 1)
    // Board constructor
{
    clear();
}

void board::clear()
// Mark all possible values as legal for each board entry
{
    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            value[i][j] = Blank;
        }
}

void board::setCell(int i, int j, int data)
{
    value[i][j] = data;
}

void board::initialize(ifstream& fin)
// Read a Sudoku board from the input file.
{
    char ch;

    clear();

    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            fin >> ch;

            // If the read char is not Blank
            if (ch != '.')
                setCell(i, j, ch - '0');   // Convert char to int
         
        }
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that i,j is in.  

    return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream& operator<<(ostream& ostr, vector<int>& v)
// Overloaded output operator for vector class.
{
    for (int i = 0; i < v.size(); i++)
        ostr << v[i] << " ";
    cout << endl;

    return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad value in setCell");

    return (getCell(i, j) == Blank);
}

void board::print()
// Prints the current board.
{
    for (int i = 1; i <= BoardSize; i++)
    {
        if ((i - 1) % SquareSize == 0)
        {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++)
        {
            if ((j - 1) % SquareSize == 0)
                cout << "|";
            if (!isBlank(i, j))
                cout << " " << getCell(i, j) << " ";
            else
                cout << "   ";
        }
        cout << "|";
        cout << endl;
    }

    cout << " -";
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

void board::setCellConflicts(int i, int j)
{
    vector<int> cellConflict(10, 0);
    int square = squareNumber(i, j);

    for (int num = 1; num <= 9; num++)
    {
        for (int col = 1; col <= BoardSize; col++)
        {
            if (value[i][col] == num && col != j)
            {
                cellConflict[num]++;
            }
        }
        for (int row = 1; row <= BoardSize; row++)
        {
            if (value[row][j] == num && row != i)
            {
                cellConflict[num]++;
            }
        }
        if (square == 1)
        {
            for (int row = 1; row < 4; row++)
            {
                for (int col = 1; col < 4; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        if (square == 2)
        {
            for (int row = 1; row < 4; row++)
            {
                for (int col = 4; col < 7; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        if (square == 3)
        {
            for (int row = 1; row < 4; row++)
            {
                for (int col = 7; col < 10; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        // NEXT ROW OF SQUARES
        if (square == 4)
        {
            for (int row = 4; row < 7; row++)
            {
                for (int col = 1; col < 4; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        if (square == 5)
        {
            for (int row = 4; row < 7; row++)
            {
                for (int col = 4; col < 7; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        if (square == 6)
        {
            for (int row = 4; row < 7; row++)
            {
                for (int col = 7; col < 10; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        // NEXT ROW OF SQUARES
        if (square == 7)
        {
            for (int row = 7; row < 10; row++)
            {
                for (int col = 1; col < 4; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        if (square == 8)
        {
            for (int row = 7; row < 10; row++)
            {
                for (int col = 4; col < 7; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
        if (square == 9)
        {
            for (int row = 7; row < 10; row++)
            {
                for (int col = 7; col < 10; col++)
                {
                    if (value[row][col] == num && row != i && col != j)
                    {
                        cellConflict[num]++;
                    }
                }
            }
        }
    }
    conflicts[i][j] = cellConflict;
}

void board::setConflicts()
{
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            setCellConflicts(i, j);
        }
    }
}

void board::printConflicts()
{
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            cout << "Conflicts Cell [" << i << "][" << j << "]: ";
            cout << conflicts[i][j];
        }
        cout << endl;
    }
}

void board::findBlank()
{
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (isBlank(i, j))
            {
                return;
            }
        }
    }
}

bool board::sudokuComplete()
{
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (isBlank(i, j))
                return false;
        }
    }
    return true;
}

void solve(board& board_pass)
{
    int i = -1;
    int j = -1;
    vector<int> conflicts;
    RecursionCount++;

    if (board_pass.sudokuComplete())
    {
        return;
    }

    board_pass.findBlank();
    board_pass.setCellConflicts(i, j);
    
    for (int n = 1; n < 10; n++)
    {
        if (conflicts[n] == 0)
        {
            board_pass.setCell(i, j, n);
            solve(board_pass);
            if (board_pass.sudokuComplete())
                return;
            board_pass.setCell(i, j, Blank);
        }
    }
}

int main()
{
    ifstream fin;

    // Read the sample grid from the file.
    string fileName = "sudoku.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }

    try
    {
        board b1(SquareSize);

        while (fin && fin.peek() != 'Z')
        {
            b1.initialize(fin);
            b1.setConflicts();
            b1.print();
            b1.printConflicts();
            solve(b1);
            if (b1.sudokuComplete())
            {
                b1.print();
                cout << "Board completed in " << RecursionCount << " recursive calls\n";
                TotalCount += RecursionCount;
                RecursionCount = 0;
            }
        }
    }
    catch (indexRangeError& ex)
    {
        cout << ex.what() << endl;
        exit(1);
    }
}

