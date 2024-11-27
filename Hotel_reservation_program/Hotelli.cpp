//Matias Syd�nmaa
//Hotellinvarausohjelma 
//Arvosana 5 

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <limits> 
#include <map> 
#include <set>

using namespace std;

//Tallentaa varauksen tiedot
struct Reservation {
	int roomNumber; //Huoneen numero
	string name; //Varaajan nimi
};

//Aliohjelma, joka arpoo luvut annetulta v�lilt�
int getRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min; //Palauttaa satunnaisluvun v�lilt� [min, max]
}

//Aliohjelma, joka tarkistaa sy�tteen ja varmistaa, ett� se on numero
int getValidInput() {
	int input;
	while (true) {
		cin >> input; //Lukee k�ytt�j�n sy�tteen
		if (cin.fail()) //Tarkistaa onko sy�tteen lukeminen ep�onnistunut
		{
			//Tyhjent�� error flagin
			cin.clear();
			//Ohittaa virheellisen sy�tteen
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid input. Please enter a number." << endl;
		}
		else {
			return input; //Palauttaa kelvollisen sy�tteen
		}
	}
}

//Aliohjelma, joka arpoo vapaan huoneen numeron
int getRandomRoom(const vector<bool>& rooms, int start, int end) {
	vector<int> availableRooms; //Vektori vapaille huoneille
	for (int i = start; i < end; i++) {
		if (!rooms[i]) { //Jos huone ei ole varattu
			availableRooms.push_back(i + 1); //Lis�� huoneen numeron vapaiden huoneiden listaan
		}
	}
	if (availableRooms.empty()) {
		return -1; //Kaikki huoneet varattu
	}
	int randomIndex = getRandomNumber(0, availableRooms.size() - 1);
	return availableRooms[randomIndex]; //Palauttaa vapaan huoneen numeron
}

//Aliohjelma yhteissumman laskemiseen randomin alennuksen mukaan
int calculateTotalPrice(int nights, int pricePerNight) {
	int discount = getRandomNumber(0, 2) * 10; //Randomi alennus 0%, 10% tai 20%
	int total = nights * pricePerNight; //Kokonaishinta ilman alennusta
	total -= total * discount / 100; //V�hent�� alennuksen kokonaishinnasta
	return total; //Kokonaishinta alennuksen j�lkeen
}

//Aliohjelma huonevarauksen hakemiseen varausnumerolla
void searchReservationNumber(const map<int, Reservation>& reservations) {
	int reservationNumber;
	cout << "Enter reservation number: ";
	reservationNumber = getValidInput();
	auto it = reservations.find(reservationNumber);
	if (it != reservations.end()) {
		cout << "Reservation found: Room " << it->second.roomNumber << ", Name: " << it->second.name << endl;
	}
	else {
		cout << "Reservation not found." << endl;
	}
}

//Aliohjelma varausten hakemiseen varaajan nimell�
void searchByName(const map<int, Reservation>& reservations) {
	string name;
	cout << "Enter name: ";
	cin.ignore();
	getline(cin, name);
	bool found = false;
	for (const auto& reservation : reservations) {
		if (reservation.second.name == name) {
			cout << "Reservation found: Number " << reservation.first << ", Room " << reservation.second.roomNumber << endl;
			found = true;
		}
	}
	if (!found) {
		cout << "Reservation not found." << endl;
	}
}

int main() {

	srand(static_cast<unsigned int>(time(0))); //Asetetaan satunnaislukugeneraattori

	int n = getRandomNumber(40, 300) * 2; //Arvotaan huoneiden m��r� (40-300)
	int singleRoomPrice = 100; //Yhden hengen huoneen hinta
	int doubleRoomPrice = 150; // Kahden hengen huoneen hinta

	vector<bool> rooms(n, false); //Vektori, joka pit�� kirjaa varatuista huoneista
	map<int, Reservation> reservations; //Pit�� kirjaa varauksista
	set<int> usedReservationNumbers; //Tarkistus k�ytetyille varausnumeroille

	cout << "Welcome to the hotel!" << endl;

	while (true) {
		//Hotellin valikko
		cout << "\n============================\n";
		cout << "Main menu: " << endl;
		cout << "1. Reserve a room." << endl;
		cout << "2. Search for a reservation." << endl;
		cout << "3. Exit." << endl;
		cout << "=============================\n";
		cout << "Select an option: ";

		int option = getValidInput();

		//Tarkistetaan k�ytt�j�n sy�te
		if (option == 1) {
			int roomType;
			cout << "Select room type: 1 for single room, 2 for double room." << endl;
			while (true) {
				roomType = getValidInput();
				if (roomType == 1 || roomType == 2) {
					break;
				}
				else {
					cout << "Invalid room type selected. Please enter 1 for single room or 2 for double room." << endl;
				}
			}

			//Arvotaan vapaa huoneen numero
			int roomNumber = (roomType == 1) ? getRandomRoom(rooms, 0, n / 2) : getRandomRoom(rooms, n / 2, n);

			//Katsotaan onko huoneita vapaana
			if (roomNumber == -1) {
				cout << "All rooms are reserved." << endl;
				continue;
			}

			//Generoidaan varausnumero ja tarkistetaan, ettei se ole jo k�yt�ss�
			int reservationNumber;
			do {
				reservationNumber = getRandomNumber(10000, 99999);
			} while (usedReservationNumbers.count(reservationNumber));

			usedReservationNumbers.insert(reservationNumber);

			//Kysyt��n varaajan nimi
			cout << "Enter your name: ";
			cin.ignore();
			string name;
			getline(cin, name);

			//P�ivitet��n huoneen ja varauksen tiedot
			rooms[roomNumber - 1] = true;
			reservations[reservationNumber] = { roomNumber, name };

			cout << "Room " << roomNumber << " is reserved for you." << endl;
			cout << "Your reservation number is " << reservationNumber << "." << endl;
			cout << "How many nights would you like to stay?" << endl;

			int nights = getValidInput();

			//Lasketaan kokonaishinta
			int pricePerNight = (roomType == 1) ? singleRoomPrice : doubleRoomPrice;
			int total = calculateTotalPrice(nights, pricePerNight);
			cout << "You are staying " << nights << " nights and the total price is " << total << " euros." << endl;

		}
		else if (option == 2) {
			//Varausten haku
			cout << "Search by: 1 for reservation number, 2 for name." << endl;
			int searchType = getValidInput();
			if (searchType == 1) {
				searchReservationNumber(reservations);
			}
			else if (searchType == 2) {
				searchByName(reservations);
			}
			else {
				cout << "Invalid search type." << endl;
			}
		}
		else if (option == 3) {
			cout << "Thanks for using the hotel reservation system!" << endl;
			break;
		}
		else {
			cout << "Invalid option. Please select 1, 2, or 3." << endl;
		}
	}

	return 0;
}
