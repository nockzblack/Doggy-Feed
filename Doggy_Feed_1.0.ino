// SE INCLUYEN LAS LIBRERÍAS NECESARIAS
#include <EEPROM.h> // Librería para utilizar la memoria Eeprom
#include <Wire.h> // Librería I2C
#include <LiquidCrystal_I2C.h> // Librería para utilizar la pantalla LCD 
#include <Wire.h> 
#include "RTClib.h" // Librería RTC DS1307
#include <Keypad.h> // Librería Keypad

// Se definen la direciones I2C
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
RTC_DS1307 RTC;

/*
 * CODIGO NECESARIO PARA EL FUNCIONAMIENTO DEL
 * KEYPAD
 * 
 * ORDEN DE CONEXION:
 * DE IZQUIERDA A DERECHA CON LOS NUMEROS DE FRENTE
 * 13,12,11,10,9,8,7,6
 */
// Constantes de las filas y columnas del Keypad
const byte filas = 4;
const byte columnas = 4;

// Pines donde se conecta el keypad
byte pinsFilas[filas] = {9,8,7,6};
byte pinsColumnas[columnas] = {13,12,11,10};

// Matriz del teclado
char teclas[filas][columnas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Inicialización del teclado
Keypad teclado = Keypad(makeKeymap(teclas), pinsFilas, pinsColumnas, filas, columnas);

// Teimpo de espera en cambio de pantalla
int espera = 2500;

// Variables para el Teclado
int numero;
char tecla;

int decima;
int unidad;

//Constantes de los relevadores
const int motor = 2;
const int buzzer = 3;

// Variables para el buen funcionamiento de la pantalla-teclado
int Ses_S = 60;
unsigned long time_Buzz;
boolean buzz = false;
boolean buzzA = false;

unsigned long milis;
unsigned long valor;
boolean var = false;

// Variables para el funcionamiento de cada porción
int valor1;
int minP1;
int segP1;
boolean P1 = false;
boolean P1A = false;

int valor2;
int minP2;
int segP2;
boolean P2 = false;
boolean P2A = false;

int valor3;
int minP3;
int segP3;
boolean P3 = false;
boolean P3A = false;


// Variables para configurar las porciones
int horas;
int mins;


// Variable para determinar los segundos que el motor debe estar encendido
int alimento;


//Variables de tiempo para configurar la hora
int hora;
int minuto;
int segundo;
int dia;
int mes;
int ano = 2016;

int preAjuste;
bool cambio = false;


void setup() {

  Serial.begin(9600);
  
  //Se definen los reles y se apagan
  pinMode(motor,OUTPUT);
  digitalWrite(motor,HIGH);
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,HIGH);

  //Inicialización del RTC
  Wire.begin(); 
  RTC.begin(); 

  //Inicialización del LCD y mensaje de inicio
  lcd.begin(20,4);
  lcd.home();
  lcd.print("BIENVENIDO A: ");
  lcd.setCursor(0,1);
  lcd.print("DOGGY FEED");
  lcd.setCursor(0,2);
  lcd.print("LA MEJOR OPCION PARA");
  lcd.setCursor(0,3);
  lcd.print("TU AMIGO DE 4 PATAS");
  delay(espera);
  lcd.clear();
}

void loop() {
  milis = millis(); // Guarda en la variable milis los milisegundos desde que se encendio la placa
  readTecla(); // Lee los valores del keypad
  reloj(); // Ejecuta la funcion reloj que es donde se muetsra la fecha en pantalla 

  if(tecla  == 'A'){
    lcd.clear();
    do{
      menuConfigurar();
      if(tecla == 'A'){
        lcd.clear();
        do{
          selecPorcion();
          if(tecla == 'A'){
            lcd.clear();
            do{ // INICIA PORCION 1
              procedimientoPorcion1();      
            } while (tecla != 'D'); // TERMINA PORCION 1
            readTecla();
            lcd.clear();
          }
          if(tecla == 'B'){
            lcd.clear();
            do{ // INICIA PORCION 2
              procedimientoPorcion2(); 
            } while (tecla != 'D'); // TERMINA PORCION 2
            readTecla();
            lcd.clear();
          }
          if(tecla == 'C'){
            lcd.clear();
            do{ // INICIA PORCION 3
              procedimientoPorcion3();
            } while (tecla != 'D'); // TERMINA PORCION 3
            readTecla();
            lcd.clear();
          }
        } while (tecla != 'D');
        readTecla();
        lcd.clear();
      }
      if(tecla == 'B'){
        lcd.clear();
        do{
          tamano();
          configTamano();
          if(tecla == 'A'){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("TAMANO CHICO");
            lcd.setCursor(0,2);
            lcd.print("SELECCIOANDO");
            delay(espera);
            tecla = 'D';
          }
          if(tecla == 'B'){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("TAMANO MEDIANO");
            lcd.setCursor(0,2);
            lcd.print("SELECCIOANDO");
            delay(espera);
            tecla = 'D';
          }
          if(tecla == 'C'){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("TAMANO GRANDE");
            lcd.setCursor(0,2);
            lcd.print("SELECCIOANDO");
            delay(espera);
            tecla = 'D';
          }
        } while (tecla != 'D');
        readTecla();
        lcd.clear();
      }
      if(tecla == 'C'){
        lcd.clear();
        do{
          procedimientoConfigHoraYFecha();
        } while (tecla != 'D');
        readTecla();
        lcd.clear();
      }
    } while (tecla != 'D');
    readTecla();
    lcd.clear();
}
}

// Función para leer la tecla del Keypad
void readTecla () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'  & tecla != '*' & tecla != '#'){
       numero = tecla - 48;
    }
}

//Función para mostrar el reloj en pantalla
void reloj () {
  if(var != 1){
  valor = milis + 1000;
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(now.hour(), DEC); // Horas
  lcd.print(':');
  lcd.print(now.minute(), DEC); // Minutos
  lcd.print(':');
  lcd.print(now.second(), DEC); // Segundos
  lcd.print("  ");
  lcd.print(now.day(), DEC); // Dia
  lcd.print('/');
  lcd.print(now.month(), DEC); // Mes
  lcd.print('/');
  lcd.print(now.year(), DEC); // Año 
  lcd.setCursor(0,2);
  lcd.print("PRESIONE A");
  lcd.setCursor(0,3);
  lcd.print("PARA CONFIGURAR");

  Serial.println(P1);
  Serial.println(P2);
  Serial.println(P3);

  Serial.println(buzz);
  Serial.println(buzzA);
  Serial.println();
  
  workPor1();
  workPor2();
  workPor3();

  if(buzz != 1 && buzzA != 1) {
    if(P1 != 0 || P2 != 0 || P3 != 0){
      time_Buzz = milis + 1000;
      digitalWrite(buzzer,LOW);
      buzz++;
    }
  }

  if(milis >= time_Buzz) {
    digitalWrite(buzzer,HIGH);
    buzz = 0;
  }
  
  var++;
  }
  
  if(milis >= valor) {
    var = 0;
  }
}


// FUNCIONES PARA TRABAJAR LA PORCION INDICADA

// PORCION 1
void workPor1(){
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC 
  if(EEPROM.read(1) == now.hour()){
    if(EEPROM.read(2) == now.minute()){
      if(P1 != 1){  
         valor1 = EEPROM.read(0);
         minP1 = valor1/Ses_S;
         segP1 = valor1 - (minP1 * 60);
         minP1 = now.minute() + minP1;
         segP1 = now.second() + segP1;      
         digitalWrite(motor,LOW);
         lcd.clear();
         lcd.setCursor(0,2);
         lcd.print("Sirviendo Porcion 1");
         delay(espera);  
         P1++;
         P1A++; 
      }
    }
  }

  /* POSIBLES FALLAS:
   *  Cuando la porcion se empieza antes de terminar una hora y termina después del minuto 1 de la sigueinte hora.
   *  Cuando se empieza a servir la porcion despues del segundo 1
   */
  
  if(minP1 <= now.minute() || now.minute() == 0 && P1A != 0){
    if(segP1 <= now.second()){
      digitalWrite(motor,HIGH);
      buzzA++;
    }
    if(minP1 != now.minute()){
        P1 = 0; 
        P1A = 0;
        buzzA = 0;
      }
  }
}

// PORCION 2
void workPor2(){
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC 
  if(EEPROM.read(3) == now.hour()){
    if(EEPROM.read(4) == now.minute()){
      if(P2 != 1){  
         valor2 = EEPROM.read(0);
         minP2 = valor2/Ses_S;
         segP2 = valor2 - (minP2 * 60);
         minP2 = now.minute() + minP2;
         segP2 = now.second() + segP2;
         digitalWrite(motor,LOW);
         lcd.clear();
         lcd.setCursor(0,2);
         lcd.print("Sirviendo Porcion 2");
         delay(espera);  
         P2++; 
         P2A++; 
      }
    }
  }

  /* POSIBLES FALLAS:
   *  Cuando la porcion se empieza antes de terminar una hora y termina después del minuto 1 de la sigueinte hora.
   *  Cuando se empieza a servir la porcion despues del segundo 1
   */
  
  if(minP2 <= now.minute() && P2A != 0){
    if(segP2 <= now.second()){
      digitalWrite(motor,HIGH);
      buzzA++;
    }
    if(minP2 != now.minute()){
        P2 = 0; 
        P2A = 0;
        buzzA = 0;
      }
  }
}

// PORCION 3
void workPor3(){
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC 
  if(EEPROM.read(5) == now.hour()){
    if(EEPROM.read(6) == now.minute()){
      if(P3 != 1){  
         valor3 = EEPROM.read(0);
         minP3 = valor3/Ses_S;
         segP3 = valor3 - (minP3 * 60);
         minP3 = now.minute() + minP3;
         segP3 = now.second() + segP3;
         digitalWrite(motor,LOW);
         lcd.clear();
         lcd.setCursor(0,2);
         lcd.print("Sirviendo Porcion 3");
         delay(espera);  
         P3++; 
         P3A++; 
      }
    }
  }

  /* POSIBLES FALLAS:
   *  Cuando la porcion se empieza antes de terminar una hora y termina después del minuto 1 de la sigueinte hora.
   *  Cuando se empieza a servir la porcion despues del segundo 1
   */
  
  if(minP3 <= now.minute() && P3A != 0){
    if(segP3 <= now.second()){
      digitalWrite(motor,HIGH);
      buzzA++;
    }
    if(minP3 != now.minute()){
        P3 = 0; 
        P3A = 0;
        buzzA = 0;
      }
  }
}

// Función para configurar el reloj
void menuConfigurar () {
  readTecla();
  lcd.setCursor(0,0);
  lcd.print("SELECCIONE: ");
  lcd.setCursor(0,1);
  lcd.print("A-> PORCIONES");
  lcd.setCursor(0,2);
  lcd.print("B-> MODIFICAR TAMANO");
  lcd.setCursor(0,3);
  lcd.print("C-> CAMBIAR LA HORA");
}

// EMPIEZAN FUNCIONES PARA LAS PORCIONES
// Función para determinar la porcion
void selecPorcion() {
  readTecla();
  lcd.setCursor(0,0);
  lcd.print("Selecione Porcion:");
  lcd.setCursor(0,1);
  lcd.print("Porcion 1 = Boton A");
  lcd.setCursor(0,2);
  lcd.print("Porcion 2 = Boton B");
  lcd.setCursor(0,3);
  lcd.print("Porcion 3 = Boton C");
}

// INICIAN FUNCIONES PARA PORCION 1 

// Procedimiento Función 1
void procedimientoPorcion1() {
  porcion1H();
  readTecla1HD();
  if(tecla == 'A'){
    lcd.clear();
    do{
      porcion1H();
      readTecla1HU();
      if(tecla == 'A'){
        lcd.clear();
        do{
          porcion1M();
          readTecla1MD();
          if(tecla == 'A'){
            lcd.clear();
            do{
              porcion1M();
              readTecla1MU();
              if(tecla == 'A'){
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Porcion1 Configurada");
                lcd.setCursor(0,2);
                lcd.print("Correctamente");
                delay(espera);
                tecla = 'D';
              }
            } while (tecla != 'D');
          }
        } while (tecla != 'D');
      }
    } while (tecla != 'D');
  }
}


// Funcion que vizualiza la porcion 1 en horas
void porcion1H() {
  readTecla();
  lcd.setCursor(5,0);
  lcd.print("PORCION 1");
  lcd.setCursor(0,1);
  lcd.print("Selecione: Horas");
  lcd.setCursor(5,2);
  lcd.print("->");
  lcd.print(EEPROM.read(1));
  lcd.print(":");
  lcd.print(EEPROM.read(2));
  lcd.setCursor(0,3);
  lcd.print("A=Aceptar D=Regresar");
}

void readTecla1HD() {
  tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'  & tecla != '*' & tecla != '#'){
       decima = tecla - 48;
       horas = decima * 10;
       EEPROM.write(1,horas);
    }
}

void readTecla1HU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'  & tecla != '*' & tecla != '#'){
       unidad = tecla - 48;
       horas = horas + unidad;
       EEPROM.write(1,horas);
    }
}


// Funcion que vizualiza la porcion 1 en minutos
void porcion1M() {
  readTecla();
  lcd.setCursor(5,0);
  lcd.print("PORCION 1");
  lcd.setCursor(0,1);
  lcd.print("Selecione: Minutos");
  lcd.setCursor(7,2);
  lcd.print(EEPROM.read(1));
  lcd.print(":");
  lcd.print(EEPROM.read(2));
  lcd.print("<-");
  lcd.setCursor(0,3);
  lcd.print("A=Aceptar D=Regresar");
}

void readTecla1MD () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'  & tecla != '*' & tecla != '#'){
       decima = tecla - 48;
       mins = decima* 10;
       EEPROM.write(2,mins);
    }
}

void readTecla1MU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'  & tecla != '*' & tecla != '#'){
       unidad = tecla - 48;
       mins = mins + unidad;
       EEPROM.write(2,mins);
    }
}
// TERMINAN FUNCIONES PARA PORCION 1
 

// INICIAN FUNCIONES PARA PORCION 2

// Procedimiento Función 2
void procedimientoPorcion2() {
  porcion2H();
  readTecla2HD();
  if(tecla == 'A'){
    lcd.clear();
    do{
      porcion2H();
      readTecla2HU();
      if(tecla == 'A'){
        lcd.clear();
        do{
          porcion2M();
          readTecla2MD();
          if(tecla == 'A'){
            lcd.clear();
            do{
              porcion2M();
              readTecla2MU();
              if(tecla == 'A'){
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Porcion2 Configurada");
                lcd.setCursor(0,2);
                lcd.print("Correctamente");
                delay(espera);
                tecla = 'D';
              }
            } while (tecla != 'D');
          }
        } while (tecla != 'D');
      }
    } while (tecla != 'D');
  }
}


//Funcion que vizualiza la porcion 2 en horas
void porcion2H() {
  readTecla();
  lcd.setCursor(5,0);
  lcd.print("PORCION 2");
  lcd.setCursor(0,1);
  lcd.print("Selecione: Horas");
  lcd.setCursor(5,2);
  lcd.print("->");
  lcd.print(EEPROM.read(3));
  lcd.print(":");
  lcd.print(EEPROM.read(4));
  lcd.setCursor(0,3);
  lcd.print("A=Aceptar D=Regresar");
}

void readTecla2HD() {
  tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
       decima = tecla - 48;
       horas = decima * 10;
       EEPROM.write(3,horas);
    }
}

void readTecla2HU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
       unidad = tecla - 48;
       horas = horas + unidad;
       EEPROM.write(3,horas);
    }
}

//Funcion que vizualiza la porcion 2 en minutos
void porcion2M() {
  readTecla();
  lcd.setCursor(5,0);
  lcd.print("PORCION 2");
  lcd.setCursor(0,1);
  lcd.print("Selecione: Minutos");
  lcd.setCursor(7,2);
  lcd.print(EEPROM.read(3));
  lcd.print(":");
  lcd.print(EEPROM.read(4));
  lcd.print("<-");
  lcd.setCursor(0,3);
  lcd.print("A=Aceptar D=Regresar");
}

void readTecla2MD () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
       decima = tecla - 48;
       mins = decima* 10;
       EEPROM.write(4,mins);
    }
}

void readTecla2MU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
       unidad = tecla - 48;
       mins = mins + unidad;
       EEPROM.write(4,mins);
    }
}
// TERMINAN FUNCIONES PARA PORCION 2



// INICIAN FUNCIONES PARA PORCION 3
// Procedimiento Función 2
void procedimientoPorcion3() {
  porcion3H();
  readTecla3HD();
  if(tecla == 'A'){
    lcd.clear();
    do{
      porcion3H();
      readTecla3HU();
      if(tecla == 'A'){
        lcd.clear();
        do{
          porcion3M();
          readTecla3MD();
          if(tecla == 'A'){
            lcd.clear();
            do{
              porcion3M();
              readTecla3MU();
              if(tecla == 'A'){
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Porcion3 Configurada");
                lcd.setCursor(0,2);
                lcd.print("Correctamente");
                delay(espera);
                tecla = 'D';
              }
            } while (tecla != 'D');
          }
        } while (tecla != 'D');
      }
    } while (tecla != 'D');
  }
}

//Funcion que vizualiza la porcion 3 en horas
void porcion3H() {
  readTecla();
  lcd.setCursor(5,0);
  lcd.print("PORCION 3");
  lcd.setCursor(0,1);
  lcd.print("Selecione: Horas");
  lcd.setCursor(5,2);
  lcd.print("->");
  lcd.print(EEPROM.read(5));
  lcd.print(":");
  lcd.print(EEPROM.read(6));
  lcd.setCursor(0,3);
  lcd.print("A=Aceptar D=Regresar");
}

void readTecla3HD() {
  tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'){
       decima = tecla - 48;
       horas = decima * 10;
       EEPROM.write(5,horas);
    }
}

void readTecla3HU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'){
       unidad = tecla - 48;
       horas = horas + unidad;
       EEPROM.write(5,horas);
    }
}

//Funcion que vizualiza la porcion 3 en minutos
void porcion3M() {
  readTecla();
  lcd.setCursor(5,0);
  lcd.print("PORCION 3");
  lcd.setCursor(0,1);
  lcd.print("Selecione: Minutos");
  lcd.setCursor(7,2);
  lcd.print(EEPROM.read(5));
  lcd.print(":");
  lcd.print(EEPROM.read(6));
  lcd.print("<-");
  lcd.setCursor(0,3);
  lcd.print("A=Aceptar D=Regresar");
}

void readTecla3MD () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'){
       decima = tecla - 48;
       mins = decima* 10;
       EEPROM.write(6,mins);
    }
}

void readTecla3MU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D'){
       unidad = tecla - 48;
       mins = mins + unidad;
       EEPROM.write(6,mins);
    }
}
// TERMINAN FUNCIONES PARA PORCION 3

// EMPIEZAN FUNCIONES PARA EL TAMAÑO DE LA MASCOTA
// Función para determinar el tamaño de la mascota
void tamano() {
  readTecla();
  lcd.setCursor(0,0);
  lcd.print("TAMANO DE LA MASCOTA");
  lcd.setCursor(0,1);
  lcd.print("CHICO = Boton A");
  lcd.setCursor(0,2);
  lcd.print("MEDIANO = Boton B");
  lcd.setCursor(0,3);
  lcd.print("GRANDE = Boton C");
}

void configTamano() {
  if(tecla == 'A'){
    alimento = 40;
    EEPROM.write(0,alimento);
  }
  if(tecla == 'B'){
    alimento = 107;
    EEPROM.write(0,alimento);
  }
  if(tecla == 'C'){
    alimento = 234;
    EEPROM.write(0,alimento);
  }
}
// TERMINAN FUNCIONES PARA EL TAMAÑO DE LA MASCOTA


// EMPIEZAN FUNCIONES PARA EL CONFIGURAR LA HORA DEL SISTEMA
void procedimientoConfigHoraYFecha() {
  configReloj();
  defineHD(); //HORA
  if(cambio == true){
    lcd.clear();
    do{
      configReloj();
      defineHU(); //HORA
      if(tecla == 'A'){
        numero = 0;
        lcd.clear();
        do{
          configReloj();
          defineMD(); //MINUTO
          RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
          if(cambio == true){
            lcd.clear();
            do{
              configReloj();
              defineMU(); //MINUTO
              RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
              if(tecla == 'A'){
                lcd.clear();
                numero = 0;
                do{
                  configReloj1();
                  defineDIAD(); //DIA
                  RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                  if(cambio == true){
                    lcd.clear();
                    do{
                      configReloj1();
                      defineDIAU(); //DIA
                      RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                      if(tecla == 'A'){
                        numero = 0;
                        lcd.clear();
                        do{
                          configReloj1();
                          defineMESD();
                          RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                          if(cambio == true){
                            lcd.clear();
                            do{
                              configReloj1();
                              defineMESU();
                              RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                              if(tecla == 'A'){
                                lcd.clear();
                                do{
                                  configReloj1();
                                  defineANOD();
                                  RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                                  if(tecla == 'A'){
                                    lcd.clear();
                                    do{
                                      configReloj1();
                                      defineANOU(); 
                                      RTC.adjust(DateTime(ano, mes, dia, hora, minuto, 0));
                                      if(tecla == 'A'){
                                        lcd.clear();
                                        lcd.setCursor(4,1);
                                        lcd.print("FECHA Y HORA");
                                        lcd.setCursor(5,2);
                                        lcd.print("CONFIGURADA");
                                        delay(espera);
                                        tecla = 'D';                                                                     
                                      }
                                    } while (tecla != 'D');
                                  }
                                } while (tecla != 'D');
                              }
                            } while (tecla != 'D');
                          }
                        } while (tecla != 'D');
                        mes = 0;
                      }
                    } while (tecla != 'D');
                  }
                } while (tecla != 'D');
                dia = 0;
              }
            } while (tecla != 'D');
            minuto = 0;
          }
        } while (tecla != 'D');
      }
    } while (tecla != 'D');
    numero = 0;
  }
  cambio = false;
}

// Función para configurar el reloj
void configReloj () {
  readTecla();
  lcd.setCursor(0,0);
  lcd.print("Configura la Hora");
  lcd.setCursor(0,1);
  lcd.print("-->");
  lcd.print(hora);
  lcd.print(":");
  lcd.print(minuto);
  lcd.print(":");
  lcd.print(segundo);
  
  lcd.setCursor(0,2);
  lcd.print("Configura la Fecha");
  lcd.setCursor(0,3);
  lcd.print(dia);
  lcd.print("/");
  lcd.print(mes);
  lcd.print("/");
  lcd.print(ano);
}

void defineHD() {
  preAjuste = hora;
  readTecla();
  hora = numero;
  if(preAjuste != hora) {
    cambio = true;   
  }
}

void defineHU() {
  tecla = teclado.getKey();
  if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
    numero = tecla - 48;
    hora = hora * 10;
    hora = hora + numero; 
    }  
}

void defineMD () {    
  preAjuste = minuto;
  readTecla();
  minuto = numero;
  if(preAjuste != minuto) {
    cambio = true;   
  }
}

void defineMU () { 
  tecla = teclado.getKey();
  if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
    numero = tecla - 48;
    minuto = minuto * 10;
    minuto = minuto + numero; 
    }   
}

// Función para configurar el reloj
void configReloj1 () {
  lcd.setCursor(0,0);
  lcd.print("Configura la Hora");
  lcd.setCursor(0,1);
  lcd.print(hora);
  lcd.print(":");
  lcd.print(minuto);
  lcd.print(":");
  lcd.print(segundo);
  
  lcd.setCursor(0,2);
  lcd.print("Configura la Fecha");
  lcd.setCursor(0,3);
  lcd.print("-->");
  lcd.print(dia);
  lcd.print("/");
  lcd.print(mes);
  lcd.print("/");
  lcd.print(ano);
}

void defineDIAD() {
  preAjuste = dia;
  readTecla();
  dia = numero;
  if(preAjuste != dia) {
    cambio = true;   
  }     
}

void defineDIAU() { 
  tecla = teclado.getKey();
  if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
    numero = tecla - 48;
    dia = dia * 10;
    dia = dia + numero; 
    }   
}

void defineMESD () {   
  preAjuste = mes;
  readTecla();
  mes = numero;
  if(preAjuste != mes) {
    cambio = true;   
  } 
}

void defineMESU () {  
  tecla = teclado.getKey();
  if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
    numero = tecla - 48;
    mes = mes * 10;
    mes = mes + numero; 
    }  
}

void defineANOD () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
       decima = tecla - 48;
       ano = decima * 10 + 2000;
    }
}

void defineANOU () {    
    tecla = teclado.getKey();
    if(tecla != NO_KEY & tecla != 'A' & tecla != 'B' & tecla != 'C' & tecla != 'D' & tecla != '*' & tecla != '#'){
       unidad = tecla - 48;
       ano = ano + unidad;
    }
}
// TERMINAN FUNCIONES PARA EL CONFIGURAR LA HORA DE LA MASCOTA

