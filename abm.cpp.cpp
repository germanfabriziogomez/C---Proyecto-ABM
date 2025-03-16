//============================================================================
// Name        : abm.cpp
// Author      : German Gomez
// Description : Aplicacion simple de ABM (Altas, Bajas, Modificaciones)
//============================================================================

#include <stdlib.h>
#include <iostream>
#include <stdio.h>


using namespace std;

struct Usuario {	//cada usuario pesa 15 bytes (11+4)

	char nombre[30];
	int dni;
};


void grabar(Usuario user)
{

	//abro el archivo, si no existe se crea, y si ya existe queda apuntando al final del archivo
	FILE* f= fopen("usuarios.dat","a+b");

	//grabo
	fwrite(&user,sizeof(Usuario),1,f);

	//cierro el archivo
	fclose(f);
}

void darAltaUsuario()
{
	Usuario user;
	cout << "Ingrese nombre:" ;
	cin.ignore();
	cin.getline(user.nombre,30,'\n');

	cout << "Ingrese DNI:";
	cin>>user.dni;


	grabar(user);
}

void borrar(int dniBuscado)
{
	//abro el archivo para lectura y escritura
	FILE* f= fopen("usuarios.dat","r+b");

	//busco al usuario
	Usuario user;
	fread(&user,sizeof(Usuario),1,f);

	//creo un archivo auxiliar
	FILE* aux= fopen("auxiliar.dat","wb");

	//mientras que el dni no sea el buscado y no termine el archivo, sigo buscando
	while(!feof(f))
	{
		//si es distinto al usuario buscado, lo grabo el el archivo auxiliar
		if(user.dni!=dniBuscado)
		{
			fwrite(&user,sizeof(Usuario),1,aux);
		}
		fread(&user,sizeof(Usuario),1,f);
	}
	//cierro los archivos
	fclose(aux);
	fclose(f);

	//borro el archivo del disco
	remove("usuarios.dat");

	//renombro el archivo auxiliar con el nombre que tenia el archivo original
	rename("auxiliar.dat","usuarios.dat");


}

void darBajaUsuario()
{

	cout << "Ingrese DNI:";
	int dni;
	cin.ignore();
	cin>>dni;
	borrar(dni);
}


void procesarModificacion(int dni, int opcion)
{
	FILE* f= fopen("usuarios.dat","r+b");

	Usuario user;
	fread(&user,sizeof(Usuario),1,f);
	while(user.dni!=dni && !feof(f))
	{
		fread(&user,sizeof(Usuario),1,f);
	}
	if(user.dni!=dni)
	{
		cout << "Error, no existen registros con el numero buscado" << endl;
	}
	else
	{
		//si la opcion fue modificar el nombre de usuario...
		if(opcion==1)
		{
			cout << "Ingrese el nuevo nombre de usuario:";
			cin.ignore();
			cin>>user.nombre;
		}

		//si la opcion fue modificar el dni...
		else
		{
			cout << "Ingrese el nuevo DNI:";
			cin.ignore();
			cin>>user.dni;
		}


		//apunto una posicion hacia atras
		fseek(f,-1*sizeof(Usuario),SEEK_CUR);

		//grabo el usuario con el nuevo dni/nombre de usuario
		fwrite(&user,sizeof(Usuario),1,f);

	}

	fclose(f);
}

void modificarUsuario()
{

	cout << "Ingrese una opcion:" << endl;
	cout << "1-Modificar nombre de usuario" << endl;
	cout << "2-Modificar DNI usuario" << endl;

	int opcion;
	cin>>opcion;

	cout << "Ingrese el dni del usuario a modificar:";
	int dni;
	cin>>dni;

	procesarModificacion(dni,opcion);

}

void leerArchivo()
{
	FILE* f= fopen("usuarios.dat","rb");

	//apunto al final del archivo
	fseek(f,0,SEEK_END);

	//obtengo la longitud del archivo
	int longitudArchivo=ftell(f)/sizeof(Usuario);

	//vuelvo el puntero al inicio
	fseek(f,0,SEEK_SET);

	//si el archivo esta vacio o si el archivo no existe...f
	if(longitudArchivo==0 || f==NULL)
	{
		cout << "Error. No hay nada para leer" << endl;
	}
	else
	{
		Usuario user;
		fread(&user,sizeof(Usuario),1,f);
		cout << "---------Lista de usuarios-----------" << endl;

		while(!feof(f))
		{

			cout << user.nombre << ", DNI " << user.dni << endl;
			fread(&user,sizeof(Usuario),1,f);
		}
		cout << "-------------------------------------" << endl;
	}
	fclose(f);
}
void menu()
{
	int opcion;

	cout << "\nIngrese una opcion:" << endl;
	cout << "1-Alta de usuario" << endl;
	cout << "2-Baja de usuario" << endl;
	cout << "3-Modificar datos del usuario"<<endl;
	cout << "4-Leer archivo"<<endl;
	cout << "5-Salir\n"<<endl;

	cin>>opcion;

	switch(opcion)
	{
		case 1:
		{
			darAltaUsuario();
			menu();
			break;
		}
		case 2:
		{
			darBajaUsuario();
			menu();
			break;
		}
		case 3:
		{
			modificarUsuario();
			menu();
			break;
		}
		case 4:
		{
			leerArchivo();
			menu();
			break;
		}
		case 5:
		{
			break;
		}
	}
}

int main() {

	menu();

	return 0;
}
