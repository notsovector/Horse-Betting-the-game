#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <time.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>

using namespace std;

int money;
//making bet
int bet=0;
int betMin = 0;
int betMax;
//menu options
int minOption = 0;
int maxOption = 3;
//choosing horses
int horse = 1;
int horseMin = 1;
int const horseMax = 6;
//countdown
int toStart = 5;
//starting/current positions (coordinates)
int horses[horseMax];
//finishing positions (places)
int currentPosition = 1;
int positions[horseMax]{};
int finishLine = 100;
//stats - tb added
double games;
double wins;
int loses;
int maxWin;
int maxLose;
int wonSum;
int lostSum;
int maxBet;
int plHorse[horseMax];
int winHorse[horseMax];


void load();
void save();
void menu();
int rng();
void race();
void menuOptionManager(int& option, bool& running);
void valueCheck(int& value, int min, int max);
void drawRace(int phase);
void writeHorses(int number);
void move(int number);
bool isFinished();
void results();
void otherOptionManager(int option);
void reset();
void statistics();

BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType) {
	switch (ctrlType) {
	case CTRL_CLOSE_EVENT: // U¿ytkownik klikn¹³ "X" (krzy¿yk) okna konsoli
		save();        // Automatyczny zapis gry!

		// Dajemy systemowi chwilê na dokoñczenie zapisu pliku.
		// Bez tego system mo¿e "zabiæ" proces zanim plik zostanie zapisany.
		Sleep(1000);

		return TRUE; // Informujemy system, ¿e obs³u¿yliœmy zdarzenie

		// Mo¿esz te¿ opcjonalnie obs³u¿yæ inne zdarzenia:
	case CTRL_C_EVENT:        // Ctrl+C
		save();
	case CTRL_BREAK_EVENT:    // Ctrl+Break
		save();
		return TRUE;

	default:
		return FALSE;
	}
}

void hideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void clearScreen() {
	COORD coord{ 0,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
	if (!SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE)) {
		return 1;
	}
	load();
	reset();
	SetConsoleTitle(TEXT("Horse Betting: The Game"));
	hideCursor();
	while (true) {
		menu();
	}
	return 0;
}

void load() {
	ifstream checkFile("save.sav");
	bool fileExists = checkFile.good();
	checkFile.close();
	if (!fileExists) {
		ofstream wrFile("save.sav");
		wrFile << 1000 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0<<" "<<0;
		wrFile.close();
	}
	ifstream rFile("save.sav");
	rFile >> money >> games >> wins >> loses >> maxWin >> maxLose >> wonSum >> lostSum >> maxBet;
	rFile.close();
}

void save() {
	ofstream wrFile("save.sav");
	wrFile << money << " " << games << " " << wins << " " << loses << " " << maxWin << " " << maxLose << " " << wonSum << " " << lostSum<<" "<<maxBet;
	wrFile.close();
}

void menu() {
	int activeOption = 0;
	if (money == 0) { //totally not scripted random event
		system("cls");
		cout << "\n\nYour balance is empty, but game doesn't end now - please accept those 100$ and bet more!\n";
		money += 100;
		betMax = money;
		if (activeOption == 0) {
			cout << ">";
		}
		else {
			cout << " ";
		}
		cout << " OK";
		otherOptionManager(activeOption);
	}
	bool running=true;
	while (running) {
		
		clearScreen();
		cout << "===HORSE BETTING: THE GAME===\n\n";
		cout << "Your money: " << money << "$\n";
		if (activeOption == 3) {
			cout << ">";
		}
		else {
			cout << " ";
		}
		cout << " STATS\n\n";
		if (activeOption == 0) {
			cout << ">";
		}
		else {
			cout << " ";
		}
		cout << " Your bet: < " << bet << "$ >   \n";
		if (activeOption == 1) {
			cout << ">";
		}
		else {
			cout << " ";
		}
		cout << " Your horse: < " << horse << " >\n";
		if (activeOption == 2) {
			cout << ">";
		}
		else {
			cout << " ";
		}
		cout << " START\n\n\n" << endl;
		cout << "Click ESC to exit" << endl;
		menuOptionManager(activeOption, running);
	}
}

int rng() {
	int randomNumber = rand() % 20;
	if (randomNumber > 5 && randomNumber < 15) {
		return 1;
	}
	else if (randomNumber >= 17) {
		return 2;
	}
	return 0;
}

void race() {
	srand(time(NULL));
	system("cls");
	hideCursor();
	for (int i = 0; i < horseMax; i++) {
		horses[i] = 1;
	}
	int moveCnt = 0;
	int phase = 1;
	//phase 1 - before race
	while (toStart > 0) {
		drawRace(1);
		toStart--;
		this_thread::sleep_for(1s);
		clearScreen();
	}
	//phase 2 - race
	while (!isFinished()) {
		for (int i = 0; i < horseMax; i++) {
			move(i);
		}
		drawRace(2);
		this_thread::sleep_for(100ms);
		clearScreen();
	}
	//results
	results();
}

void valueCheck(int& value, int min, int max) {
	if (value > max) {
		value = min;
	}
	if (value < min) {
		value = max;
	}
}

void menuOptionManager(int& option, bool& running) {
	int key = _getch();
	int activeOption = 0;
	if (key == 27) {
		save();
		exit(0);
	}

	if (key == 13) {
		if (option == 2) { //start
			if (bet == 0) {
				system("cls");
				cout << "\n\n\nYou can't bet 0$! That's not a charity organisation.\n";
				if (activeOption == 0) {
					cout << ">";
				}
				else {
					cout << " ";
				}
				cout << " OK\n";
				otherOptionManager(activeOption);
			}
			else {
				running = false;
				race();
			}
		}
		else if (option == 3) { //statistics
			statistics();
		}
		return;
	}
	switch (option) {
	case 0: //bet
		if (key == 0 || key == 224) {
			key = _getch();
			switch (key) {
			case 72:
				option--;
				valueCheck(option, minOption, maxOption);
				break;
			case 75:
				bet -= 5;
				valueCheck(bet, betMin, betMax);
				break;
			case 77:
				bet += 5;
				valueCheck(bet, betMin, betMax);
				break;
			case 80:
				option++;
				valueCheck(option, minOption, maxOption);
				break;
			}
		}
		break;
	case 1: //horse
		if (key == 0 || key == 224) {
			key = _getch();
			switch (key) {
			case 72:
				option--;
				valueCheck(option, minOption, maxOption);
				break;
			case 75:
				horse--;
				valueCheck(horse, horseMin, horseMax);
				break;
			case 77:
				horse++;
				valueCheck(horse, horseMin, horseMax);
				break;
			case 80:
				option++;
				valueCheck(option, minOption, maxOption);
				break;
			}
		}
		break;
	case 2: //start
		if (key == 0 || key == 224) {
			key = _getch();
			switch (key) {
			case 72:
				option--;
				valueCheck(option, minOption, maxOption);
				break;
			case 80:
				option++;
				valueCheck(option, minOption, maxOption);
				break;
			}
		}
		break;
	case 3:
		if (key == 0 || key == 224) {
			key = _getch();
			switch (key) {
			case 72:
				option--;
				valueCheck(option, minOption, maxOption);
				break;
			case 80:
				option++;
				valueCheck(option, minOption, maxOption);
				break;
			}
		}
	}
}

void drawRace(int phase) {
	cout << "===RACE DAY===\n\n";

	if (phase == 1) {
		cout << "Race begins in: " << toStart << " seconds\n";
	}
	else {
		cout << "Race ongoing             \n";
	}
	
	for (int i = 0; i < horseMax; i++) {
		writeHorses(i);
	}
}

void writeHorses(int number) {
	cout << (number+1);
	for (int i = 0; i < finishLine; i++) {
		if (i == horses[number]) {
			cout << "X";
		}
		else {
			cout << " ";
		}
	}
	switch (number) {
	case 0:
		cout << "F";
		break;
	case 1:
		cout << "I";
		break;
	case 2:
		cout << "N";
		break;
	case 3:
		cout << "I";
		break;
	case 4:
		cout << "S";
		break;
	case 5:
		cout << "H";
		break;
	}
	cout << " ";
	if (positions[number] != 0) {
		cout << positions[number];
	}
	cout << endl;
}

void move(int number) {
	if (positions[number] == 0) {
		horses[number] += rng();
		if (horses[number] >= finishLine) {
			positions[number] = currentPosition;
			currentPosition++;
		}
	}
	return;
}

bool isFinished() {
	for (int i = 0; i < 6; i++) {
		if (positions[i] == 0) {
			return false;
		}
	}
	return true;
}

void results() {
	int activeOption = 0;
	games++;
	system("cls");
	hideCursor();
	cout << "Your bet: " << bet << "$" << endl;
	cout << "Your horse: " << horse << endl << endl;
	cout << "==================================\n\n";
	cout << "Your horse has placed: " << positions[horse - 1];
	switch (positions[horse - 1]) {
	case 1:
		cout << "st";
		break;
	case 2:
		cout << "nd";
		break;
	case 3:
		cout << "rd";
		break;
	default:
		cout << "th";
	}
	cout << endl;
	cout << "Your winning: ";
	if (positions[horse - 1] == 1) {
		money += bet;
		cout << bet << "$\n";
		wins++;
		if (bet > maxWin) {
			maxWin = bet;
		}
		wonSum += bet;
	}
	else {
		money -= bet;
		cout << "0$\n";
		loses++;
		lostSum += bet;
		if (bet>maxLose) {
			maxLose = bet;
		}
	}
	cout << "\nYour current balance: "<<money<<"$\n\n\n";
	if (activeOption == 0) {
		cout << ">";
	}
	else {
		cout << " ";
	}
	cout << " BACK\n";
	//place for other future options
	otherOptionManager(activeOption);
}

void otherOptionManager(int option) {
	int key = _getch();

	if (key == 13) {
		if (option == 0) {
			reset();
			system("cls");
		}
		return;
	}
}

void reset() {
	bet = 0;
	horse = 1;
	toStart = 5;
	currentPosition = 1;
	for (int i = 0; i < horseMax; i++) {
		positions[i] = 0;
	}
	betMax = money;
}

void statistics() {
	system("cls");
	double winrate = 0;
	int activeOption = 0;
	if (games != 0) {
		winrate = 100 * wins / games;
	}
	cout << "===STATISTICS===\n\n";
	cout << "Games played: " << games << endl;
	cout << "Games won: " << wins << endl;
	cout << "Games lost: " << loses << endl;
	cout << fixed << setprecision(2);
	cout << "Winrate: " << winrate << "%\n";
	cout << fixed << setprecision(0);
	cout << "Biggest bet: " << maxBet << "$\n";
	cout << "Biggest win: " << maxWin << "$\n";
	cout << "Biggest lose: " << maxLose << "$\n";
	cout << "Won money: " << wonSum << "$\n";
	cout << "Lost money: " << lostSum << "$\n\n\n";
	if (activeOption == 0) {
		cout << ">";
	}
	else {
		cout << " ";
	}
	cout << " BACK\n";
	otherOptionManager(activeOption);
}