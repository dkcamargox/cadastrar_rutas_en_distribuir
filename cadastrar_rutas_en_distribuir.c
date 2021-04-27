#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>


#define X 950
#define Y 0
#define SCREEN_WIDTH 1365
#define SCREEN_HEIGHT 767

#define APAGA VK_VOLTAR
#define ENTER VK_RETURN
#define TAB VK_TAB

void await(int tempo) {
	//temporizador
    clock_t start_time = clock();
    while (clock() < start_time + tempo); 
    return;
}

void FullScreen() {  
	keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c , 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

void MouseSetup(INPUT *buffer) {
    buffer->type = INPUT_MOUSE;
    buffer->mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
    buffer->mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
    buffer->mi.mouseData = 0;
    buffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
    buffer->mi.time = 0;
    buffer->mi.dwExtraInfo = 0;
}

void MouseMoveAbsolute(INPUT *buffer, int x, int y) {
    buffer->mi.dx = (x * (0xFFFF / SCREEN_WIDTH));
    buffer->mi.dy = (y * (0xFFFF / SCREEN_HEIGHT));
    buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);

    SendInput(1, buffer, sizeof(INPUT));
}

void MouseClick(INPUT *buffer) {
    buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
    SendInput(1, buffer, sizeof(INPUT));

    Sleep(10);

    buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
    SendInput(1, buffer, sizeof(INPUT));
}

void MouseDoubleClick(INPUT *buffer) {
	MouseClick(buffer);
	MouseClick(buffer);
}

void MouseClickAbsolute(INPUT *buffer, int x, int y) {
	MouseMoveAbsolute(buffer, x, y);
	MouseClick(buffer);
	return;
}

void MouseDoubleClickAbsolute(INPUT *buffer, int x, int y) {
	MouseMoveAbsolute(buffer, x, y);
	MouseDoubleClick(buffer);
	return;
}

void KeyPress(char key) {
	INPUT ip;
	// Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    // Press the "A" key
    ip.ki.wVk = VkKeyScan(key); 
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
}

void KeypressCtrlC() {
	// Create a generic keyboard event structure
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Press the "V" key
    ip.ki.wVk = 'C';
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "V" key
    ip.ki.wVk = 'C';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

void KeyPressString(char string[]) {
	int i;
	
	for( i = 0 ; i < strlen(string) ; i++) {
		KeyPress(string[i]);
		await(100);
	}
}

void PrintIntArray(int *IntArray, int ArraySize) {
	int size = sizeof(IntArray) / sizeof(int);
	int i;
	for (i = 0; i < ArraySize ; i++) {
		printf("%d - %d\n",i , IntArray[i]);
	}
}

void BailOut(char *msg) {
  fprintf(stderr, "Exiting: %s\n", msg);
  exit(1);
}

char* GetFromClipboard() {
  HANDLE h;
  if (!OpenClipboard(NULL)) BailOut("Can't open clipboard");
  h = GetClipboardData(CF_TEXT);
  CloseClipboard();
  return (char *)h;
}

void ConfigureSchema(INPUT *buffer) {
	//move to schema select
	MouseClickAbsolute(buffer, 633, 154);
	//select refrigerados schema
	MouseClickAbsolute(buffer, 633, 182);
	
}
void ChangeIdentificationType(INPUT *buffer) {
	//checking for not cuit
	MouseDoubleClickAbsolute(buffer, 1015, 232);
	KeyPressString("96");
	KeyPress(ENTER);
	await(600);
	//changing DNI
	MouseDoubleClickAbsolute(buffer, 880, 250);
	KeyPressString("11111111");
	KeyPress(ENTER);
	await(600);
	
}
void InputClientInDistribuirByRoute(INPUT *buffer, int client_code, int route_code, int distribuition_route_code) {
	
	//move and click to client selection
	char str[21];
	char identification_type[200];
	MouseDoubleClickAbsolute(buffer, 795, 152);
	//parsing to string
	sprintf(str, "%d", client_code);
	//wrinting the client code
	KeyPressString(str);
	//rewinding auxiliar string
	strcpy(str, "\0");
	//strinking enter
	KeyPress(ENTER);
	await(600);
	//going to edit button
	MouseClickAbsolute(buffer, 240, 115);
	//waiting distribuir to respond
	await(2000);
	//checking for not cuit
	MouseDoubleClickAbsolute(buffer, 1015, 232);
	await(100);
	//copy tipo identf
	KeypressCtrlC();
	await(100);
	
	//read clipboard
	strcpy(identification_type, GetFromClipboard());
	if (!strcmp(identification_type, "99")) ChangeIdentificationType(buffer);
	
	//going to second edit button
	MouseClickAbsolute(buffer, 225, 545);
	//waiting distribuir to respond
	await(2000);
	
	//changin sell route
	MouseDoubleClickAbsolute(buffer, 340, 570);
	await(100);
	//parsing to string
	sprintf(str, "%d", route_code);
	//wrinting the route code
	KeyPressString(str);
	//rewinding auxiliar string
	strcpy(str, "\0");
	//hit enter
	KeyPress(ENTER);
	
	//changin distribution route
	MouseDoubleClickAbsolute(buffer, 340, 620);
	await(100);
	//parsing to string
	sprintf(str, "%d", route_code);
	//wrinting the distribution route code
	KeyPressString(str);
	//rewinding auxiliar string
	strcpy(str, "\0");
	//hit enter
	KeyPress(ENTER);
	
	
	//going to first save button
	MouseClickAbsolute(buffer, 220, 590);
	//waiting distribuir to respond
	await(2000);
	
	//going to second save button
	MouseClickAbsolute(buffer, 315, 115);
	//waiting distribuir to respond
	await(2000);
	
	printf("\tCliente:\t%d\t\tRuta de Venda:\t%d\t\tRuta de distribuicion:\t%d\n", client_code, route_code, distribuition_route_code );
	return;
}

int main(int argc, char *argv[])
{
 	FILE *file;
 	int client_code;
 	int *clients_codes; 
 	int route_code;
 	int distribuition_route_code;
 	int i;
 	int count_clients = 0;
 	INPUT buffer;
 	
 	//setup mouse
    MouseSetup(&buffer);
 	//prompt setup
 	system("color 17");
	
	system("cls");

	//making file
 	file = fopen("clientes_ruta.txt", "at");
 	fclose(file);
	// first manual
	printf("\n\n\t\t\tCadastrar Rutas en Distribuir - REDCOM S.A.\n\n");
 	printf("\tUSAJE:\n\n");
	printf("\tAbri el archivo clientes_rutas.txt");
	printf("\n\tEn un bloc de notas escribir los dados seguiendo las seguintes espicificaciones\n\n");
	printf("\t--------------------------------------------------------------------------------\n\n");
	printf("\t<RUTA DE VENDA> (PRIMERO VALOR EN EL ARCHIVO)\n");
	printf("\t<RUTA DE DISTRIBUICION> (SEGUNDO VALOR EN EL ARCHIVO)\n");
	printf("\t<CLIENTE 1>\n");
	printf("\t<CLIENTE 2>\n");
	printf("\t<CLIENTE 31>\n");
	printf("\t<CLIENTE 4>\n");
	printf("\t<CLIENTE 5>\n");
	printf("\t<CLIENTE 6>\n");
	printf("\t<CLIENTE 7>\n");
	printf("\t<CLIENTE 8>\n");
	printf("\t...\n");
	printf("\t--------------------------------------------------------------------------------\n\n");
	printf("\tNo pongas ningun otro caracter como TAB o ENTER en el archivo, eso puede prejudicar\n\tel funcionamento del programa\n\n");
	printf("\tProbablemente la computadora estara fuera de uso mientras el programa ejecuta\n\n");
	printf("\tSi el archivo ya esta listo, click en ese prompt y pusa cualquier tecla\n\n");
	getch();
	//opening file
	file = fopen("clientes_ruta.txt", "r");
 	//scaning first element => route
	fscanf(file, "%d", &route_code);
	//scaning second element => dist route
	fscanf(file, "%d", &distribuition_route_code);
	//scanning rest of elements => clients codes
	for (i=1 ; !feof(file) ; i++, count_clients++) {
		fscanf(file, "%d", &client_code);
		if( i == 1) clients_codes = malloc(i * sizeof(int));
		else clients_codes = realloc(clients_codes, i * sizeof(int));
		clients_codes[i-1] = client_code;
		printf("a");
	}
	
	//second manual
	system("cls");
	printf("\n\n\t\t\tCadastrar Rutas en Distribuir - REDCOM S.A.\n\n");
	printf("\tAbri el maestro de clientes en modo pantalla completa (fullscreen).\n\n");
	printf("\tDespues volve en ese prompt y pulsa cualquier tecla iniciaremos el proceso.\n\n");
	getch();
	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	
	system("cls");
	//configure schema
	ConfigureSchema(&buffer);
	//insertion
	printf("\n\t\t\t\t\t\tClientes Inseridos:\n\n");
	for (i=0 ; i < count_clients ; i++) {
		InputClientInDistribuirByRoute(&buffer, clients_codes[i], route_code, distribuition_route_code);
	}
	getch();
	
	system("cls");
	printf("\n\n\t\t\tCadastrar Rutas en Distribuir - REDCOM S.A.\n\n");
	printf("\tDesarollado por Douglas Camargo para REDCOM S.A. con licencia MIT.\n\n\tCualquier problema o consulta escribi a camargo.douglas@icloud.com\n");
	printf("\n\tMuchas Gracias! Pulsa cualquier tecla para finalizar.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//close file
	fclose(file);
	//free allocated array
	if (count_clients >= 1) free(clients_codes);
	//ending it all
	return 0;
}
