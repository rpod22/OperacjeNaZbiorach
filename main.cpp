#include <iostream>

class Element {
public:
    int wartosc;
    Element* poprzedni; //wskaznik na poprzedni element w liscie dwukierunkowej
    Element* nastepny; //wskaznik na nastepny element w liscie dwukierunkowej
    Element(int wartosc) : wartosc(wartosc), poprzedni(nullptr), nastepny(nullptr) {} //Konstruktor inicjujacy wartosc elementu
};

class ListaDwukierunkowa {
private:
    Element* glowa; //wskaznik na glowe listy
    Element* ogon;  //wskaznik na ogon listy
public:
    ListaDwukierunkowa() : glowa(nullptr), ogon(nullptr) {} //konstruktor inicujacy glowe i ogon jako nullptr

    void dodaj(int wartosc) { //metoda dodaj nowy element do listy
        Element* nowyElement = new Element(wartosc); //tworzy nowy element
        if (!glowa) { //jesli lista jest pusta
            glowa = ogon = nowyElement; // ustawia zarowno glowe i ogon na nowy element
        } else {
            ogon->nastepny = nowyElement; // jesli nie to ustawia nastepny element ostatniego elementu jako nowy element
            nowyElement->poprzedni = ogon; //ustawia poprzedni element nowego elementu na poprzedni ogon
            ogon = nowyElement; // aktualizuje ogon na nowy element
        }
    }

    void usun(int wartosc) { //metoda usuwania elementu o danej wartosci z listy
        Element* obecny = glowa; //rozpoczyna od glowy listy
        while (obecny) { //dopoki nie dojdzie do konca listy
            if (obecny->wartosc == wartosc) { //jesli wartosc obecnego elementu jest rowna podanej wartosci
                if (obecny->poprzedni) { //jesli obecny element ma poprzednika
                    obecny->poprzedni->nastepny = obecny->nastepny; // aktualizuje nastepnika poprzednika na nastepnika obencego
                } else {
                    glowa = obecny->nastepny; // jesli nie ma poprzednika ustawia glowe na nastepny element
                }
                if (obecny->nastepny) { // jesli obecny ma nastepnika
                    obecny->nastepny->poprzedni = obecny->poprzedni; //aktualizuje poprzednika nastepnika na poprzednika obecnego
                } else {
                    ogon = obecny->poprzedni; // jesli nie ma nastepnika ustawia ogon na poprzedni element
                }
                delete obecny; //usuwa obecny element
                return;
            }
            obecny = obecny->nastepny; //przechodzi do nastepnego elementu (wejdzie do petli tylko gdy znajdzie element o podanej wartosci)
        }
    }

    bool czyZawiera(int wartosc) const { //metoda sprawdza czy lista zawiera element o danej wartosci
        Element* obecny = glowa; //start od glowy
        while (obecny) { //dopoki nie dojdzie do konca listy
            if (obecny->wartosc == wartosc) { //jesli wartosc elementu jest taka jak podana
                return true; //wartosc znaleziona
            }
            obecny = obecny->nastepny; //przechodzi do nastepnego elementu
        }
        return false; //wartosc nie znaleziona
    }

    friend class Zbior; //zaprzyjaznienie z klasa Zbior
};

class Zbior {
private:
    ListaDwukierunkowa lista; // lista przechowujaca zbiory elementy zbioru
public:
    void dodaj(int wartosc) { //metoda dodawania elementu jesli nie istnieje jeszcze w zbiorze
        if (!lista.czyZawiera(wartosc)) {
            lista.dodaj(wartosc);
        }
    }

    //usuwanie elementu ze zbioru
    void usun(int wartosc) {
        lista.usun(wartosc);
    }

    //metoda obliczania sumy zbiorow
    Zbior suma(const Zbior& inny) const {
        Zbior wynik;
        Element* obecny = lista.glowa; //rozpoczyna od glowy listy w biezacym zbiorze
        while (obecny) { 
            wynik.dodaj(obecny->wartosc); //dodaj wartosc obecnego elementu do wynikowego zbioru
            obecny = obecny->nastepny; //przechodzi do nastepnego elementu
        }
        obecny = inny.lista.glowa; 
        while (obecny) {
            wynik.dodaj(obecny->wartosc);  //to samo dla drugiego zbioru
            obecny = obecny->nastepny;
        }
        return wynik;
    }

    //metoda do obliczania iloczynu zbiorow
    Zbior iloczyn(const Zbior& inny) const {
        Zbior wynik;
        Element* obecny = lista.glowa;
        while (obecny) {
            if (inny.lista.czyZawiera(obecny->wartosc)) { //jesli drugi zbior zawiera wartosc obecnego elementu
                wynik.dodaj(obecny->wartosc); //dodaj wartosc obecnego elementu do zbioru wynikowego
            }
            obecny = obecny->nastepny; //przejscie do nastepengo elementu
        }
        return wynik;
    }

    //metoda do obliczania roznicy zbiorow
    Zbior roznica(const Zbior& inny) const {
        Zbior wynik;
        Element* obecny = lista.glowa;
        while (obecny) {
            if (!inny.lista.czyZawiera(obecny->wartosc)) { //jesli drugi zbior nie zawiera wartosci obecnego elementu
                wynik.dodaj(obecny->wartosc); //dodaj wartosc obecnego elementu do zbioru wynikowego
            }
            obecny = obecny->nastepny; //przejscie do nastpnego ekementu
        }
        return wynik;
    }

    //metoda sprawdzajaca czy zbior zawiera wszystkie elementy innego zbioru
    bool czyZawiera(const Zbior& inny) const {
        Element* obecny = inny.lista.glowa;
        while (obecny) {
            if (!lista.czyZawiera(obecny->wartosc)) { //jesli biezacy zbior nie zawiera wartosci obecnego elementu
                return false; //false bo nie zawiera wszystkich elementow
            }
            obecny = obecny->nastepny;
        }
        return true; //true bo zbior zawiera wszystkie elementy 
    }

    //metoda sprawdzajaca czy biezacy zbior wynika z innego zbioru
    bool wynikanie(const Zbior& inny) const {
        return inny.czyZawiera(*this); // wywoluje metode czyZawiera na zbiorze drugim z biezacym jako argumentem
    }

    //metoda wyswietlajaca zawartosc biezacego zbioru
    void wyswietl() const {
        Element* obecny = lista.glowa; 
        std::cout << "{ ";
        while (obecny) {
            std::cout << obecny->wartosc << " "; //drukowanie wartosci obecnego elementu
            obecny = obecny->nastepny; //przejscie do nastepnego
        }
        std::cout << "}" << std::endl;
    }
};

class Parser {
private:
    Zbior zbiory[26]; //Tablica zbiorow od A do Z 

    //metoda wykonujaca operacje na dwoch zbiorach na podstawie podanego operatora
    Zbior wykonajOperacje(char operacja, Zbior& lewy, Zbior& prawy) {
        switch (operacja) {
        case '+':
            return lewy.suma(prawy); //dodawanie
        case '*':
            return lewy.iloczyn(prawy); //mnozenie
        case '-':
            return lewy.roznica(prawy); //odejmowanie
        case '<':
            if (lewy.czyZawiera(prawy)) { //zawieranie
                return lewy;  
            }
            break;
        case '>':
            if (lewy.wynikanie(prawy)) { //wynikanie
                return prawy;
            }
            break;
        }
        return Zbior(); //zwrocenie pustego zbioru dla niepoprawnie wpisanej operacji
    }

public:
    //metoda dodawania zbioru o podanej nazwie
    void dodajZbior(char nazwa, const Zbior& zbior) {
        zbiory[nazwa - 'A'] = zbior; //oblicza indeks tablicy ktory odpowiada nazwie zbioru (z tabeli ASCII) i dodaje zbior do tablicy
        //zgodnie z odpowiadajacym mu indeksem
    }

    //metoda usuwania zbioru o podanej nazwie
    void usunZbior(char nazwa) {
        zbiory[nazwa - 'A'] = Zbior(); //oblicza indeks na tej samej zasadzie i zastepuje zbior pustym zbiorem
    }

    //metoda wykonujaca operacje na zbiorach na podstawie podanego wyrazenia
    Zbior wykonajWyrazenie(const std::string& wyrazenie) {
        Zbior lewy = zbiory[wyrazenie[0] - 'A']; //pobiera lewy zbior na podstawie pierwszego znaku wyrazenia
        Zbior prawy;
        for (size_t i = 1; i < wyrazenie.size(); i += 2) { //przechodzi przez kolejne znaki wyrazenia, zaczynajac od drugiego
            char operacja = wyrazenie[i];   
            prawy = zbiory[wyrazenie[i + 1] - 'A']; //pobiera prawy zbior na podstawie nastepnego znaku 
            lewy = wykonajOperacje(operacja, lewy, prawy);
        }
        return lewy; //wynik
    }
};

void wyswietlOpcje() {
    std::cout << "Wybierz opcje:" << std::endl;
    std::cout << "1. Dodaj zbior" << std::endl;
    std::cout << "2. Usun zbior" << std::endl;
    std::cout << "3. Wyswietl zawartosc zbioru" << std::endl;
    std::cout << "4. Wykonaj wyrazenie" << std::endl;
    std::cout << "5. Wyjdz" << std::endl;
}

void interfejs() {
    Parser parser; //obiekt do obslugi polecen
    std::string komenda;

    while (true) {
        wyswietlOpcje();
        std::cout << "Wpisz numer opcji: ";
        std::getline(std::cin, komenda); //pobranie polecenia

        if (komenda == "5") {
            break;
        }

        if (komenda == "1") {
            char nazwa;
            std::string zbior;
            std::cout << "Podaj nazwe zbioru: ";
            std::cin >> nazwa;
            std::cout << "Podaj elementy zbioru oddzielone przecinkami (np. 1,2,3): ";
            std::cin >> zbior;
            Zbior z;
            size_t pos = 0; //zmienna dla pozycji speratatora
            while ((pos = zbior.find(',')) != std::string::npos) {  //dopoki znajduje separator
                int element = std::stoi(zbior.substr(0, pos)); //konwertowanie na liczbe
                z.dodaj(element); //dodaje liczbe do zbioru
                zbior.erase(0, pos + 1); //usuwa przetworzona czesc z separatorem
            }
            z.dodaj(std::stoi(zbior)); //dodaje ostatnia liczbe do zbioru
            parser.dodajZbior(nazwa, z); //dodaje zbior do parsera
        } else if (komenda == "2") { //usuwanie zbioru
            char nazwa;
            std::cout << "Podaj nazwe zbioru do usuniecia: ";
            std::cin >> nazwa;
            parser.usunZbior(nazwa);
        } else if (komenda == "3") { 
            char nazwa;
            std::cout << "Podaj nazwe zbioru do wyswietlenia: ";
            std::cin >> nazwa;
            parser.wykonajWyrazenie(std::string(1, nazwa)).wyswietl(); //wyswietl zawartosc zbioru
        } else if (komenda == "4") { //wykonaj wyrazenie
            std::string wyrazenie;
            std::cout << "Podaj wyrazenie do wykonania: ";
            std::cin >> wyrazenie;
            Zbior wynik = parser.wykonajWyrazenie(wyrazenie);
            wynik.wyswietl();
        } else {
            std::cout << "Nieprawidlowa opcja" << std::endl;
        }
    }
}

int main() {
    interfejs();
    return 0;
}
