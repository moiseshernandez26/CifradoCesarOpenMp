#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>

using namespace std;

int main()
{
    int resp= 3; // Variable para las respuestas del usuario
    const int n = 4; // Desplazamientos para realizar la encriptacion
    const int CARACTERESMAX = 300; // Caracteres maximos para escribir en el texto
    string nomArchivo;
    int cifrado; // Variable para realizar el cifrado
    int caracter;
    char texto[CARACTERESMAX]; // cadena para guardar el texto escrito por el usuario
    char respGuardar;
    string llave; // variable para guardar la llave escrita por el usuario
    string indices;
    string ruta;
    do
    {
        system("cls");
        cout << "\t\tPrograma que realiza la encriptacion de un texto o archivo de texto" << endl;
        cout << "1) Encriptar documento" << endl
            << "2) Descencriptar documento" << endl
            << "3) Encriptar texto" << endl
            << "4) Salir" << endl
            << "Tecla el numero y despues presiona <ENTER>: ";
        cin >> resp;
        if (resp == 1)
        {
            system("cls");
            cin.ignore();
            cout << "Escribe ruta y nombre del archivo que desea encriptar y despues presiona <ENTER>: ";
            getline(cin, nomArchivo);
            ifstream archivo(nomArchivo);
            if (archivo.fail())
                cerr << "Error al abrir el archivo" << endl;
            else
            {
                cout << "Escribe la ruta en donde quieres guardar el archivo: ";
                getline(cin, ruta);
                #pragma omp parallel // Programacion paralela
                {
                    indices = "";
                    while (!archivo.eof())
                    {
                        caracter = archivo.get();
                        indices += (char)caracter;
                        if (archivo.eof())
                        {
                            // Condicional necesaria para no guardar el caracter de finalizacion del archivo
                        }
                        else
                        {
                            cifrado = (caracter + n); // Se hace el cifrado

                            // Se crea el archivo cifrado
                            ofstream archTemp(ruta + "/archivoEncriptado.txt", ios::app);
                            archTemp << (char)cifrado;
                            archTemp.close();
                        }
                    }
                    archivo.close();
                }
                cout << "Archivo encriptado exitosamente" << endl;
                cout << "Llave para desencriptar: " << to_string(n) + indices[0] << endl;
                system("pause");
            }

        }
        else if (resp == 2)
        {
            system("cls");
            cin.ignore();
            cout << "Escribe la ruta y nombre del archivo que desea desencriptar y despues presiona <ENTER>: ";
            getline(cin, nomArchivo);
            ifstream archivo(nomArchivo);
            if (archivo.fail())
                cerr << "Error al abrir el archivo" << endl;
            else
            {
                char llaveCifrada[2];
                cout << "Escribe la llave: ";
                getline(cin, llave);

                // Se extrae el primer caracter del archivo
                llaveCifrada[0] = (char)archivo.get();
                llaveCifrada[1] = '\0';
                archivo.close();

                // Si las llaves son iguales entonces desencripta el archivo
                if (llave == to_string(n) + string(llaveCifrada))
                {
                    archivo.open(nomArchivo);
                    cout << "Escribe la ruta en donde quieres guardar el archivo: ";
                    getline(cin, ruta);
                    #pragma omp parallel
                    {
                        while (!archivo.eof())
                        {
                            caracter = archivo.get();
                            if (archivo.eof())
                            {
                            }
                            else
                            {
                                // Se desencripta el archivo
                                cifrado = (caracter - n);
                                ofstream archTemp(ruta + "/archivoDesencriptado.txt", ios::app);
                                archTemp << (char)cifrado;
                                archTemp.close();
                            }
                        }
                        archivo.close();
                    }
                    cout << "Archivo desencriptado exitosamente" << endl;
                    system("pause");
                }
                else
                {
                    cout << "Llave incorrecta" << endl;
                    system("pause");
                }
            }
        }
        else if (resp == 3)
        {
            system("cls");
            cout << "\t\tEscribe el texto que deseas encriptar\nPara finalizar teclea el caracter \"|\" y despues presiona <ENTER>" << endl;
            cin.getline(texto, CARACTERESMAX,'|');
            string encriptar = "";
            #pragma omp parallel
            {
                #pragma omp for
                for (int i = 0; texto[i] != '\0'; i++)
                {
                    // Se encripta el texto
                    encriptar += (char)((int)texto[i] + n);
                }
            }
            cout << "Texto encriptado exitosamente" << endl;
            string textoEncriptado = encriptar.substr(1, encriptar.length());
            cout << "Deseas guardarlo en un archivo (S/N): ";
            cin >> respGuardar;
            if (respGuardar == 's')
            {
                cin.ignore();
                cout << "Escribe la ruta en donde quieres guardar el archivo: ";
                getline(cin, ruta);
                ofstream archTemp(ruta + "/textoEncriptado.txt");
                archTemp << textoEncriptado;
                archTemp.close();
                cout << "Creacion del archivo exitosamente" << endl;
                cout << "Llave para desencriptar: " << to_string(n) + textoEncriptado[0] << endl;
                system("pause");
            }
            

        }
    } while (resp != 4);
    return 0;
}

