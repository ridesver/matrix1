#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void files()
{
	ofstream fout;
	fout.open("A.txt");
	fout << "A.txt\n3, 3\n2 2 2\n2 2 2\n2 2 2";
	fout.close();

	fout.open("B.txt");
	fout << "B.txt\n3, 3\n1 1 1\n1 1 1\n1 1 1";
	fout.close();

	fout.open("C.txt");
	fout << "C.txt\n3, 3\n1 2 3\n4 5 6\n7 8 9";
	fout.close();
}

class matrix_t
{
	int **data;
	unsigned int rows, columns;

public:

	matrix_t(unsigned int rows = 0, unsigned int columns = 0)
	{
		data = new int *[rows];
		for (unsigned int i = 0; i < rows; ++i)
		{
			data[i] = new int[columns];
			for (unsigned int j = 0; j < columns; ++j)
				data[i][j] = 0;
		}
		this->rows = rows;
		this->columns = columns;
	}

	matrix_t(const matrix_t & other)
	{
		data = new int *[other.rows];
		for (unsigned int i = 0; i < other.rows; ++i)
		{
			data[i] = new int[other.columns];
			for (unsigned int j = 0; j < other.columns; ++j)
				data[i][j] = other.data[i][j];
		}
		this->rows = other.rows;
		this->columns = other.columns;
	}

	~matrix_t()
	{
		for (unsigned int i = 0; i < rows; i++)
			delete[] data[i];
		delete[] data;
	}

	matrix_t add(const matrix_t & other) const
	{
		matrix_t result(rows, columns);
		for (unsigned int i = 0; i < rows; ++i)
		{
			for (unsigned int j = 0; j < columns; ++j)
				result.data[i][j] = data[i][j] + other.data[i][j];
		}
		return result;
	}

	matrix_t sub(const matrix_t & other) const
	{
		matrix_t result(rows, columns);
		for (unsigned int i = 0; i < rows; ++i)
		{
			for (unsigned int j = 0; j < columns; ++j)
				result.data[i][j] = data[i][j] - other.data[i][j];
		}
		return result;
	}

	matrix_t mul(const matrix_t & other) const
	{
		matrix_t result(rows, other.columns);
		for (unsigned int i = 0; i < rows; ++i)
		{
			for (unsigned int j = 0; j < other.columns; ++j)
			{
				result.data[i][j] = 0;
				for (int k = 0; k < columns; k++)
					result.data[i][j] += (data[i][k] * other.data[k][j]);
			}
		}
		return result;
	}

	matrix_t trans() const
	{
		matrix_t result(columns, rows);
		for (unsigned int i = 0; i < columns; ++i)
		{
			for (unsigned int j = 0; j < rows; ++j)
				result.data[i][j] = data[j][i];
		}
		return result;
	}

	ifstream & read(ifstream & stream)
	{
		string str1, str2;
		getline(stream, str1);
		int rows, columns;
		char z;
		if (!(stream >> rows && stream >> z && z == ',' && stream >> columns))
		{
			stream.setstate(ios::failbit);
			return stream;
		}
		this->rows = rows;
		this->columns = columns;

		int **new_data = new int *[rows];
		for (unsigned int i = 0; i < rows; ++i)
		{
			new_data[i] = new int[columns];
			getline(stream, str2);
			for (unsigned int j = 0; j < columns; ++j)
			{
				if (!(stream >> new_data[i][j]))
				{
					stream.setstate(ios::failbit);
					return stream;
				}
			}
		}

		data = new int *[rows];
		for (unsigned int i = 0; i < rows; ++i)
		{
			data[i] = new int[columns];
			for (unsigned int j = 0; j < columns; ++j)
				data[i][j] = new_data[i][j];
		}

		for (unsigned int i = 0; i < rows; ++i)
			delete[] new_data[i];
		delete[] new_data;
		return stream;
	}

	ostream & write(ostream & stream)
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
				stream << data[i][j] << ' ';
			stream << endl;
		}
		return stream;
	}
};

bool read(istream & stream, matrix_t & matrix)
{
	string file;
	if (stream >> file)
	{
		ifstream fin;
		fin.open(file);
		if ((fin.is_open()) && (matrix.read(fin)))
			return true;
	}
	return false;
}

int main()
{
	files();
	matrix_t a;
	char op;
	if (read(cin, a) && cin >> op)
	{
		if (op == '+' || op == '-' || op == '*')
		{
			matrix_t b;
			if (read(cin, b))
			{
				if (op == '+')
				{
					matrix_t result = a.add(b);
					result.write(cout);
				}
				else if (op == '-')
				{
					matrix_t result = a.sub(b);
					result.write(cout);
				}
				else if (op == '*')
				{
					matrix_t result = a.mul(b);
					result.write(cout);
				}
			}
		}
		else if (op == 'T')
		{
			matrix_t result = a.trans();
			result.write(cout);
		}
	}
	else cout << "An error has occured while reading input data.\n";
	return 0;
}
