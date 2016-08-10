#include <MicroView.h>
#include <IRremote.h>

#define FREQ_KHZ 38

#define VAL_MIN 0
#define VAL_MAX 5000

#define ARR_SIZE_MAX 70

#define PIN_LDR A0

#define LDR_THRES 850

IRsend irsend;

unsigned int irSignal[ARR_SIZE_MAX];

int sensorValue;

void displayNumber(int num, bool found) {
  uView.clear(PAGE);
  uView.setCursor(0,0);
  uView.println(num);
  if(found){
    uView.print("Found!!!");
  }
  uView.display();
}

void printArray(unsigned int arr[], int length);


void recurseSubArray(unsigned int arr[], int arrSize, int startPosition) {

  if (startPosition == arrSize) {
    sendSignalData(arr, arrSize);
    sensorValue = analogRead(PIN_LDR);
    if(sensorValue > 850){
      Serial.println("Found");
      printArray(arr, arrSize);
      displayNumber(arrSize, true);
      while(true);

    }
    return;
  }


  for (int currValueInCell = VAL_MIN; currValueInCell <= VAL_MAX; currValueInCell += 5) {

    arr[startPosition] = currValueInCell;

    recurseSubArray(arr, arrSize, startPosition + 1);
  }


}

void setup() {
  pinMode(PIN_LDR, INPUT); 
  
  Serial.begin(115200);

  while (!Serial) {
  }

  

  uView.begin();    // begin of MicroView
  uView.clear(ALL); // erase hardware memory inside the OLED controller
  uView.display();  // display the content in the buffer memory, by default it is the MicroView logo
  Serial.println("Started");


  for (int arrSize = 0; arrSize < ARR_SIZE_MAX; arrSize++) {
    printArray(irSignal, arrSize);
    displayNumber(arrSize, false);
    recurseSubArray(irSignal, arrSize, 0);
  }

  Serial.println("End");
}

void loop() {





}

void sendSignalData(unsigned int arr[], int length) {
  irsend.sendRaw(irSignal, length, FREQ_KHZ);
}


void printArray(unsigned int arr[], int length) {
  for (int i = 0; i < length; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }

  Serial.println();
}


