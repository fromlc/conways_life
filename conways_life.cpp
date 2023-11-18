//------------------------------------------------------------------------------
// conways_life.cpp
// 
// Rules for Conway's Game of Life:
//		- Cells with fewer than 2 neighbors die of loneliness
//		- Cells with 4 or more neighbors die of overcrowding
//		- Cells with exactly 3 neighbors keep living or come back to life
//
// Specify starting pattern with:
//		'l' for line,
//		's' for star,
//		'r' for square.
//		't' for triangle.
//------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <windows.h>		// for SetConsoleCursorPosition()

using namespace std;

// local constants
constexpr size_t MAX = 20;

constexpr char ALIVE = '*';
constexpr char DEAD = ' ';

constexpr int X_OFFSET = 0;
constexpr int Y_OFFSET = 3;
constexpr int LINE_PATTERN = 0;
constexpr int SQUARE_PATTERN = 1;
constexpr int STAR_PATTERN = 2;
constexpr int TRIANGLE_PATTERN = 3;

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
void init_life1(int pattern);
void display_header();
void display_separator();
int reset_gen(int x, int y);
int count_life1_neighbors(int row, int col);
void generate_life2();
void display_life1();
void set_cursor_pos(int x, int y);

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
// life1[] and life2[] are MAX by MAX cells plus:
//		-one extra row on top and bottom
//		-one extra column on left and right
// extra rows and columns always contain DEADs
// to simplify counting neighbors
//
namespace g {

	char life1[MAX + 2][MAX + 2];
	char life2[MAX + 2][MAX + 2];
}

//------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------
int main() {

	// for user input, assume line pattern
	string str = "l";
	int pattern { };

	// fill first array with DEAD chars
	memset(g::life1, DEAD, (MAX + 2) * (MAX + 2));

	// copy first array into second array
	memcpy(g::life2, g::life1, (MAX + 2) * (MAX + 2));

	// display game title and top separator
	display_header();

	// track generation number for display
	int gen_num = 1;

	// play the game one generation at at time
	do {

		set_cursor_pos(0, MAX + Y_OFFSET + 2);

		// set initial pattern in first array
		switch (str[0]) {
		case 'l':
			cout << "Line Pattern";
			init_life1(LINE_PATTERN);
			gen_num = reset_gen(12, 1);
			break;
		case 's':
			cout << "Star Pattern";
			init_life1(STAR_PATTERN);
			gen_num = reset_gen(12, 1);
			break;
		case 'r':
			cout << "Square Pattern";
			init_life1(SQUARE_PATTERN);
			gen_num = reset_gen(12, 1);
			break;
		case 't':
			cout << "Triangle Pattern";
			init_life1(TRIANGLE_PATTERN);
			gen_num = reset_gen(12, 1);
			break;
//		default:
		}

		// display generation number 
		set_cursor_pos(12, 1);
		cout << gen_num++;

		// display current generation in first array
		display_life1();

		// put next generation into second array
		generate_life2();

		// copy next generation into first array
		memcpy(g::life1, g::life2, MAX * MAX);

		// bottom separator
		set_cursor_pos(0, MAX + Y_OFFSET);
		display_separator();

		// user input
		set_cursor_pos(0, MAX + Y_OFFSET + 1);
		cout << "Type <Enter> for next gen, l,s,r,t for new pattern, q quits: ";
		getline(cin, str);

	} while (str[0] != 'q' && str[0] != 'Q');

	cout << endl;

	return 0;
}

//------------------------------------------------------------------------------
// initialize the first array to starting pattern
//------------------------------------------------------------------------------
void init_life1(int pattern) {

	// reset main array to blanks
	memset(g::life1, DEAD, (MAX + 2) * (MAX + 2));

	// line pattern
	if (pattern == LINE_PATTERN) {
		g::life1[10][8] = ALIVE;
		g::life1[10][9] = ALIVE;
		g::life1[10][10] = ALIVE;
		g::life1[10][11] = ALIVE;
	}
	else if (pattern == STAR_PATTERN) {
		g::life1[10][8] = ALIVE;
		g::life1[11][8] = ALIVE;
		g::life1[11][9] = ALIVE;
		g::life1[12][9] = ALIVE;
	}
	else if (pattern == SQUARE_PATTERN) {
		g::life1[10][8] = ALIVE;
		g::life1[10][9] = ALIVE;
		g::life1[11][8] = ALIVE;
		g::life1[11][9] = ALIVE;
	}
	else if (pattern == TRIANGLE_PATTERN) {
		g::life1[10][9] = ALIVE;
		g::life1[11][8] = ALIVE;
		g::life1[11][9] = ALIVE;
		g::life1[11][10] = ALIVE;
	}
}

//------------------------------------------------------------------------------
// display a line of dashes - caller sets cursor position
//------------------------------------------------------------------------------
void display_header() {

	set_cursor_pos(0, 0);
	cout << "Game of Life";
	set_cursor_pos(0, 1);
	cout << "Generation: ";

	// separator line above
	set_cursor_pos(0, 2);
	display_separator();
}


//------------------------------------------------------------------------------
// display a line of dashes - caller sets cursor position
//------------------------------------------------------------------------------
void display_separator() {

	for (int i = 0; i < MAX; i++)
		cout << '-';
}

//------------------------------------------------------------------------------
// reset generation number to 1 and prepare console for display
//------------------------------------------------------------------------------
int reset_gen(int x, int y) {
	set_cursor_pos(x, y);
	cout << "      ";

	return 1;
}

//------------------------------------------------------------------------------
// count the neighbors of a cell in the global 2-d array
//------------------------------------------------------------------------------
int count_life1_neighbors(int row, int col) {

	int count = 0;

	// 3 top neighbors
	count += (int)g::life1[row - 1][col + 1] == ALIVE;
	count += (int)g::life1[row - 1][col - 1] == ALIVE;
	count += (int)g::life1[row - 1][col] == ALIVE;

	// 2 side neighbors
	count += (int)g::life1[row][col - 1] == ALIVE;
	count += (int)g::life1[row][col + 1] == ALIVE;

	// 3 bottom neighbors
	count += (int)g::life1[row + 1][col - 1] == ALIVE;
	count += (int)g::life1[row + 1][col] == ALIVE;
	count += (int)g::life1[row + 1][col + 1] == ALIVE;

	return count;
}

//------------------------------------------------------------------------------
// calculate one generation of life from first array into second array
//------------------------------------------------------------------------------
void generate_life2() {

	int row, col, neighbors;

	// initialize array with blanks
	memset(g::life2, DEAD, (MAX + 2) * (MAX + 2));

	for (row = 1; row <= MAX; row++) {
		for (col = 1; col <= MAX; col++) {
			// count neighbors
			neighbors = count_life1_neighbors(row, col);

			// live :-) or die of loneliness or overcrowding :-(
			if (neighbors == 2 || neighbors == 3)
				g::life2[row][col] = ALIVE;
			else
				g::life2[row][col] = DEAD;
		}
	}
}

//------------------------------------------------------------------------------
// display one generation of life
//------------------------------------------------------------------------------
void display_life1() {

	for (int row = 1; row <= MAX; row++) {

		// output asterisk for live cells, DEAD otherwise
		for (int col = 1; col <= MAX; col++) {

			set_cursor_pos(col + X_OFFSET, row + Y_OFFSET);
			cout << g::life1[row][col];
		}
	}
}

//------------------------------------------------------------------------------
// set console cursor position
//------------------------------------------------------------------------------
void set_cursor_pos(int x, int y) {
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (short)x, (short)y };

	SetConsoleCursorPosition(output, pos);
}

