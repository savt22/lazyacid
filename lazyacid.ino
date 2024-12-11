#include "Wire.h"
#include <uClock.h>
#include "MCP4725.h"

MCP4725 MCP(0x62);

float trigstart;
float trigstop;

int rootnotemap;
int scalemap;
int bpm = 120;
int bpmmap;
int clockstep;
int startstep;
int gatemult = 3;
int gatetick;
int gatetick1;
int gatetick2;
int gatetick3;
int gatetick4;
int gatetick5;
int gatetick6;
int gatetick7;
int gatetick8;
int gatetick9;
int gatetick10;
int gatetick11;
int gatetick12;
int gatetick13;
int gatetick14;
int gatetick15;
int gatetick16;
int steptick;
int steptick1;
int steptick2;
int steptick3;
int steptick4;
int steptick5;
int steptick6;
int steptick7;
int steptick8;
int steptick9;
int steptick10;
int steptick11;
int steptick12;
int steptick13;
int steptick14;
int steptick15;
int steptick16;

int exclockswitch = 1;
int exclocktick = 1;
int exclockontimer;
int exclockofftimer;

int timearray[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

//control variables

int octave;
int octavenote = -12; //sets midi note 36 to 0v
int noteadjust = -36;

int rootnote = 0;

int scale = 0;

int currentnote = 48;
int note16 = 48;
int note15 = 48;
int note14 = 48;
int note13 = 48;
int note12 = 48;
int note11 = 48;
int note10 = 48;
int note9 = 48;
int note8 = 48;
int note7 = 48;
int note6 = 48;
int note5 = 48;
int note4 = 48;
int note3 = 48;
int note2 = 48;
int note1 = 48;
int note16map = 48;
int note15map = 48;
int note14map = 48;
int note13map = 48;
int note12map = 48;
int note11map = 48;
int note10map = 48;
int note9map = 48;
int note1map = 48;
int note2map = 48;
int note3map = 48;
int note4map = 48;
int note5map = 48;
int note6map = 48;
int note7map = 48;
int note8map = 48;

int notemap = 0;
int timemap = 5;
int time1 = 16;
int time2 = 16;
int time3 = 16;
int time4 = 16;
int time5 = 16;
int time6 = 16;
int time7 = 16;
int time8 = 16;
int time9 = 16;
int time10 = 16;
int time11 = 16;
int time12 = 16;
int time13 = 16;
int time14 = 16;
int time15 = 16;
int time16 = 16;
int t1;
int t2;
int t3; 
int t4;
int t5;
int t6;
int t7;
int t8;
int t9;
int t10;
int t11; 
int t12;
int t13;
int t14;
int t15;
int t16;
int time1map;
int time2map;
int time3map;
int time4map;
int time5map;
int time6map;
int time7map;
int time8map;
int time9map;
int time10map;
int time11map;
int time12map;
int time13map;
int time14map;
int time15map;
int time16map;

const int *notearray = NULL;

static const int majorscale[21] = {48,50,52,53,55,57,59,60,62,64,65,67,69,71,72,74,76,77,79,81,83};
static const int lydianscale[21] = {48,50,52,54,55,57,59,60,62,64,66,67,69,71,72,74,76,78,79,81,83};
static const int mixolydianscale[21] = {48,50,52,53,55,57,58,60,62,64,65,67,69,70,72,74,76,77,79,81,82};
static const int minorscale[21] = {48,50,51,53,55,56,58,60,62,63,65,67,68,70,72,74,75,77,79,80,82};
static const int dorianscale[21] = {48,50,51,53,55,57,58,60,62,63,65,67,69,70,72,74,75,77,79,81,82};
static const int phrygianscale[21] = {48,49,51,53,55,56,58,60,61,63,65,67,68,70,72,73,75,77,79,80,82};
static const int chromaticscale[21] = {48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68};

//pins
const int clock_led = 10;
const int gate = 13;
const int clock_in = 9; 
const int onoff = 11;
const int clock_mode = 8;
const int bpmpot = A2;
const int scalepot = A1;
const int rootpot = A0;
const int randomswitch = 12;
int randomswitchrun = 1;

//on/off

int offswitch = 1;

//states
enum notes {
  N1,
  N2,
  N3,
  N4,
  N5,
  N6,
  N7,
  N8,
  N9,
  N10,
  N11,
  N12,
  N13,
  N14,
  N15,
  N16,
  OFF
};

notes note, prior_note;

enum clocks {
  ON,
  ON2,
  CLOCKOFF
};

clocks clockswitch, priorclockswitch;

static const float dac_preset[60] = {0.000, 0.083, 0.167, 0.250, 0.333, 0.417, 0.500, 0.583, 0.667, 0.750, 0.833, 0.917, 1.000, 1.083, 1.167, 1.250, 1.333, 1.417, 1.500, 1.583, 1.667, 1.750, 1.833, 1.917, 2.000, 2.083, 2.167, 2.250, 2.333, 2.417, 2.500, 2.583, 2.667, 2.750, 2.833, 2.917, 3.000, 3.083, 3.167, 3.250, 3.333, 3.417, 3.500, 3.583, 3.667, 3.750, 3.833, 3.917, 4.000, 4.083, 4.167, 4.250, 4.333, 4.417, 4.500, 4.583, 4.667, 4.750, 4.833, 4.917};
static const float vref = 5.00; //measure your 5v reference feeding your dac v in using a multimeter and change 5.00 to your measured value.

//functions

void on() {
  if (steptick - startstep == 1) {
    prior_note = OFF;
    note = N1;
    priorclockswitch = CLOCKOFF;
    clockswitch = ON;
    offswitch = 1;
  }
}

void off() {
  if (offswitch == 1) {
    steptick = 0;
    gatetick = 0;
    offswitch = 0;
    digitalWrite(gate, LOW);
  }
  startstep = steptick;
}

void onStepCallback(uint32_t step) {
  if ((digitalRead(onoff) == HIGH) && (digitalRead(clock_mode) == LOW)) {
    ++steptick;
  }
}

void onPPQNCallback(uint32_t tick) {
  if ((digitalRead(onoff) == HIGH) && (digitalRead(clock_mode) == LOW)) {
    ++gatetick;
  }
}

void n1() {
  if (note != prior_note) {
    steptick1 = steptick;
    gatetick1 = gatetick;
    t1 = time1;
    prior_note = note; 
    note1 = notearray[note1map];
    currentnote = (note1 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick1 == t1) {
    note = N2;
  }

  trigger();

  if (gatetick - gatetick1 == (t1*gatemult)){ //gatemult=3 sets the length of the gate to 50% of step length. 
    digitalWrite(gate, LOW);
  }
}

void n2() {
  if (note != prior_note) {
    steptick2 = steptick;
    gatetick2 = gatetick;
    t2 = time2;
    prior_note = note; 
    note2 = notearray[note2map];
    currentnote = (note2 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick2 == t2) {
    note = N3;
  }

  trigger();

  if (gatetick - gatetick2 == (t2*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n3() {
  if (note != prior_note) {
    steptick3 = steptick;
    gatetick3 = gatetick;
    t3 = time3;
    prior_note = note; 
    note3 = notearray[note3map];
    currentnote = (note3 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick3 == t3) {
    note = N4;
  }

  trigger();

  if (gatetick - gatetick3 == (t3*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n4() {
  if (note != prior_note) {
    steptick4 = steptick;
    gatetick4 = gatetick;
    t4 = time4;
    prior_note = note; 
    note4 = notearray[note4map];
    currentnote = (note4 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick4 == t4) {
    note = N5;
  }

  trigger();

  if (gatetick - gatetick4 == (t4*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n5() {
  if (note != prior_note) {
    steptick5 = steptick;
    gatetick5 = gatetick;
    t5 = time5;
    prior_note = note; 
    note5 = notearray[note5map];
    currentnote = (note5 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick5 == t5) {
    note = N6;
  }

  trigger();

  if (gatetick - gatetick5 == (t5*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n6() {
  if (note != prior_note) {
    steptick6 = steptick;
    gatetick6 = gatetick;
    t6 = time6;
    prior_note = note; 
    note6 = notearray[note6map];
    currentnote = (note6 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick6 == t6) {
    note = N7;
  }

  trigger();

  if (gatetick - gatetick6 == (t6*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n7() {
  if (note != prior_note) {
    steptick7 = steptick;
    gatetick7 = gatetick;
    t7 = time7;
    prior_note = note; 
    note7 = notearray[note7map];
    currentnote = (note7 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick7 == t7) {
    note = N8;
  }

  trigger();

  if (gatetick - gatetick7 == (t7*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n8() {
  if (note != prior_note) {  
    steptick8 = steptick;
    gatetick8 = gatetick;
    t8 = time8;
    prior_note = note; 
    note8 = notearray[note8map];
    currentnote = (note8 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick8 == t8) {
    note = N9;
  }

  trigger();

  if (gatetick - gatetick8 == (t8*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n9() {
  if (note != prior_note) {  
    steptick9 = steptick;
    gatetick9 = gatetick;
    t9 = time9;
    prior_note = note; 
    note9 = notearray[note9map];
    currentnote = (note9 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick9 == t9) {
    note = N10;
  }

  trigger();

  if (gatetick - gatetick9 == (t9*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n10() {
  if (note != prior_note) {  
    steptick10 = steptick;
    gatetick10 = gatetick;
    t10 = time10;
    prior_note = note; 
    note10 = notearray[note10map];
    currentnote = (note10 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick10 == t10) {
    note = N11;
  }

  trigger();

  if (gatetick - gatetick10 == (t10*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n11() {
  if (note != prior_note) {  
    steptick11 = steptick;
    gatetick11 = gatetick;
    t11 = time11;
    prior_note = note; 
    note11 = notearray[note11map];
    currentnote = (note11 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick11 == t11) {
    note = N12;
  }

  trigger();

  if (gatetick - gatetick11 == (t11*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n12() {
  if (note != prior_note) {  
    steptick12 = steptick;
    gatetick12 = gatetick;
    t12 = time12;
    prior_note = note; 
    note12 = notearray[note12map];
    currentnote = (note12 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick12 == t12) {
    note = N13;
  }

  trigger();

  if (gatetick - gatetick12 == (t12*gatemult)){
    digitalWrite(gate, LOW);
  }

}

void n13() {
  if (note != prior_note) {  
    steptick13 = steptick;
    gatetick13 = gatetick;
    t13 = time13;
    prior_note = note; 
    note13 = notearray[note13map];
    currentnote = (note13 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick13 == t13) {
    note = N14;
  }

  trigger();

  if (gatetick - gatetick13 == (t13*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n14() {
  if (note != prior_note) {  
    steptick14 = steptick;
    gatetick14 = gatetick;
    t14 = time14;
    prior_note = note; 
    note14 = notearray[note14map];
    currentnote = (note14 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick14 == t14) {
    note = N15;
  }

  trigger();

  if (gatetick - gatetick14 == (t14*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n15() {
  if (note != prior_note) {  
    steptick15 = steptick;
    gatetick15 = gatetick;
    t15 = time15;
    prior_note = note; 
    note15 = notearray[note15map];
    currentnote = (note15 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick15 == t15) {
    note = N16;
  }

  trigger();

  if (gatetick - gatetick15 == (t15*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void n16() {
  if (note != prior_note) {  
    steptick16 = steptick;
    gatetick16 = gatetick;
    t16 = time16;
    prior_note = note; 
    note16 = notearray[note16map];
    currentnote = (note16 + octavenote + rootnote + noteadjust);
    MCP.setValue((dac_preset[currentnote])*(4096/vref));
    digitalWrite(gate, HIGH);
    trigstart = millis();
  }
  
  if (steptick - steptick16 == t16) {
    note = N1;
  }

  trigger();

  if (gatetick - gatetick16 == (t16*gatemult)){
    digitalWrite(gate, LOW);
  }
}

void clockledon() {

  if (clockswitch != priorclockswitch) {
    clockstep = steptick;
    priorclockswitch = clockswitch;
    digitalWrite(clock_led, HIGH);
  }
  
  if (steptick - clockstep == 1) {
    clockswitch = ON2;
  }

  if (clockswitch != priorclockswitch){
    digitalWrite(clock_led, LOW);
  }
}

void trigger() {

  if(digitalRead(clock_mode) == HIGH) {
    trigstop = millis();
    if(trigstop - trigstart > 10) {
      digitalWrite(gate, LOW);
    }
  }
}

void clockledon2() {

  if (clockswitch != priorclockswitch) {
    clockstep = steptick;
    priorclockswitch = clockswitch;
    digitalWrite(clock_led, LOW);
  }
  
  if (steptick - clockstep == 1) {
    clockswitch = ON;
  }

  if (clockswitch != priorclockswitch) {
    digitalWrite(clock_led, HIGH);
  }
}

void randomgen () {
  if(digitalRead(randomswitch) == HIGH) {
    if (randomswitchrun == 1) {
      note16map = random(0, 14);
      note15map = random(0, 14);
      note14map = random(0, 14);
      note13map = random(0, 14);
      note12map = random(0, 14);
      note11map = random(0, 14);
      note10map = random(0, 14);
      note9map = random(0, 14);
      note8map = random(0, 14);
      note7map = random(0, 14);
      note6map = random(0, 14);
      note5map = random(0, 14);
      note4map = random(0, 14);
      note3map = random(0, 14);
      note2map = random(0, 14);
      note1map = random(0, 14);
      time16map = random(0, 3);
      time16 = timearray[time16map];
      time15map = random(0, 3);
      time15 = timearray[time15map];
      time14map = random(0, 3);
      time14 = timearray[time14map];
      time13map = random(0, 3);
      time13 = timearray[time13map];
      time12map = random(0, 3);
      time12 = timearray[time12map];
      time11map = random(0, 3);
      time11 = timearray[time11map];
      time10map = random(0, 3);
      time10 = timearray[time10map];
      time9map = random(0, 3);
      time9 = timearray[time9map];
      time8map = random(0, 3);
      time8 = timearray[time8map];
      time7map = random(0, 3);
      time7 = timearray[time7map];
      time6map = random(0, 3);
      time6 = timearray[time6map];
      time5map = random(0, 3);
      time5 = timearray[time5map];
      time4map = random(0, 3);
      time4 = timearray[time4map];
      time3map = random(0, 3);
      time3 = timearray[time3map];
      time2map = random(0, 3);
      time2 = timearray[time2map];
      time1map = random(0, 3);
      time1 = timearray[time1map];
      randomswitchrun = 0;
    }
  } else {
    if (randomswitchrun == 0) {
      note16map = 7;
      note15map = 6;
      note14map = 5;
      note13map = 4;
      note12map = 3;
      note11map = 2;
      note10map = 1;
      note9map = 0;
      note8map = 7;
      note7map = 6;
      note6map = 5;
      note5map = 4;
      note4map = 3;
      note3map = 2;
      note2map = 1;
      note1map = 0;
      time16map = 2;
      time16 = timearray[time16map];
      time15map = 2;
      time15 = timearray[time15map];
      time14map = 2;
      time14 = timearray[time14map];
      time13map = 2;
      time13 = timearray[time13map];
      time12map = 2;
      time12 = timearray[time12map];
      time11map = 2;
      time11 = timearray[time11map];
      time10map = 2;
      time10 = timearray[time10map];
      time9map = 2;
      time9 = timearray[time9map];
      time8map = 2;
      time8 = timearray[time8map];
      time7map = 2;
      time7 = timearray[time7map];
      time6map = 2;
      time6 = timearray[time6map];
      time5map = 2;
      time5 = timearray[time5map];
      time4map = 2;
      time4 = timearray[time4map];
      time3map = 2;
      time3 = timearray[time3map];
      time2map = 2;
      time2 = timearray[time2map];
      time1map = 2;
      time1 = timearray[time1map];
      randomswitchrun = 1;
    }
  }
}

void controls () {

  bpmmap = analogRead(bpmpot);
  bpm = map(bpmmap, 0, 1023, 40, 200);
  uClock.setTempo(bpm);

  rootnotemap = analogRead(rootpot);
  rootnote = map(rootnotemap, 0, 1023, 0, 11);

  scalemap = analogRead(scalepot);
  scale = map(scalemap, 0, 1023, 0, 7);
  switch (scale) {
    case 0: //major
      notearray = majorscale;
      break;
    case 1: //lydian
      notearray = lydianscale;
      break;
    case 2: // mixolydian
      notearray = mixolydianscale;
      break;
    case 3: //minor
      notearray = minorscale;
    case 4: //dorian
      notearray = dorianscale;
      break;
    case 5: //phrygian
      notearray = phrygianscale;
      break;
    case 6: //chromatic (poly mode won't sound nice...)
      notearray = chromaticscale;
      break;
    case 7:
      notearray = chromaticscale;
      break;
  }

  if (digitalRead(clock_mode) == LOW) {
    if (digitalRead(onoff) == HIGH) {
      on();
    } else {
      off();
    }
    if (exclockswitch == 0) {
      digitalWrite(gate, LOW);
      off();
      uClock.start();
      exclockswitch = 1;
      exclocktick = 1;
    }
  } else {
    if (digitalRead(clock_in) == HIGH) {
      if (exclockswitch == 1) {
        uClock.stop();
        prior_note = OFF;
        note = N1;
        priorclockswitch = CLOCKOFF;
        clockswitch = ON;
        offswitch = 1;
        exclockswitch = 0;
      }
      if (exclocktick == 1) {
        ++steptick;
        exclocktick = 0;
      }
      exclockontimer = millis();  
    } else {
      exclockofftimer = millis();
      if (exclocktick == 0) {
        ++steptick;
        exclocktick = 1;
      }
    }
  }

  if (exclockofftimer - exclockontimer > 500) {
    if (exclockswitch == 0) {
      prior_note = N16;
      note = OFF;
      digitalWrite(gate, LOW);
      uClock.start();
      exclockswitch = 1;
      exclocktick = 1;
    }
  }

}

void setup() {

  Wire.begin();
  MCP.begin();
  Wire.setClock(800000);

  MCP.setValue(4095);

  randomSeed(analogRead(A3));

  pinMode(clock_led, OUTPUT);
  pinMode(gate, OUTPUT);
  pinMode(clock_in, INPUT);
  pinMode(onoff, INPUT);
  pinMode(clock_mode, INPUT);
  pinMode(bpmpot, INPUT);
  pinMode(scalepot, INPUT);
  pinMode(rootpot, INPUT);
  pinMode(randomswitch, INPUT);

  prior_note = N16;
  note = OFF;
  priorclockswitch = CLOCKOFF;
  clockswitch = ON;
  notearray = majorscale;
  
  uClock.init();
  uClock.setPPQN(uClock.PPQN_96);
  uClock.setOnPPQN(onPPQNCallback);
  uClock.setOnStep(onStepCallback);
  uClock.setTempo(120);
  uClock.start();

}

void loop() {

  //state switches
  switch (note) {
    case N1:
      n1();
      break;
    case N2:
      n2();
      break;
    case N3:
      n3();
      break;
    case N4:
      n4();
      break;
    case N5:
      n5();
      break;
    case N6:
      n6();
      break;
    case N7:
      n7();
      break;    
    case N8:
      n8();
      break;
    case N9:
      n9();
      break;
    case N10:
      n10();
      break;
    case N11:
      n11();
      break;
    case N12:
      n12();
      break;
    case N13:
      n13();
      break;
    case N14:
      n14();
      break;
    case N15:
      n15();
      break;
    case N16:
      n16();
      break;
    case OFF:
      off();
      break;
  }

  switch (clockswitch) {
    case ON:
      clockledon();
      break;
    case ON2:
      clockledon2();
      break;
    case CLOCKOFF:
      digitalWrite(clock_led, LOW);
      break;
  }
  
  randomgen();
  controls();

}
