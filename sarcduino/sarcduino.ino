/*************************************************************************
* SARCduino 
* Version: Beta 0.22
* By Michael Gurevich, Javier Jaimovich, Miguel Ortiz and Nicholas Ward 
* https://github.com/jaimovier/SARCduino
*************************************************************************/

#define SAMPLE_PERIOD 10        // Sample Period in Milliseconds (this has an error of +- 2%)
                                // N.B. The arduino can sample all pins at 500Hz 
                                // This however causes serious burstiness
                                // Default is 100Hz (Same as HID devices)
                                // This avoids burstiness for most applications,
                                // Only change this if you really need a faster SR.
                                // Choose number of analog inputs to enable (0=OFF, 1=ON)
int setPins[] = {1, 1, 1, 1, 1, 1}; // Set no of analog pins e.g. {1, 0, 1, 1, 0, 0} enables pins 0, 2 and 3.
//*************--0--1--2--3--4--5
                            
// FOR REALTIME CONTROL (lower latency and burstiness)                               
// To avoid data being buffered and sent in blocks of 4KB you should insure that
// ((ANALOG_INPUTS_ENABLED * 2) + 3) * ((1/SAMPLE_PERIOD)*1000) < 3875
// This is a limitation of the ftdi usb driver
// Further details and possible solutions can be found here
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1170939903/15
// Default Sampling frequency is 100Hz (every 10ms)

//**************************************************************************//
//** Variables for data polling
//**
int analogPin, digitalPin;    // Counters for poll loop
int analogData;            // outgoing ADC value
byte x = 0; //byte to store digital status
byte y = 0; //same as above to store remaining digital pins status

//**************************************************************************//
//** Setup
//**
void setup() {
  //setup serial baudrate for USB connection
  Serial.begin(57600); 
  

             
  //enable digital pullups. All digital pins are set high
  //and will only output change when connected to ground
  for(digitalPin=2;digitalPin<14;++digitalPin) 
       {
        pinMode(digitalPin, INPUT_PULLUP);     //Setpin up for input, enable internal pullups
       }
}


//**************************************************************************//
//** Main Loop. 
//** Millis is an interrupt driven timer that begins to increment every ms once the
//** arduino starts. If millis modded with sample period is 0 then the correct sampling
//** period has passed and its time to sample again.
void loop() { 

long periodCheck = millis()%SAMPLE_PERIOD;

if (periodCheck == 0)
  {
    SampleAndSend();
  }
}

//**************************************************************************//
//** SampleAndSend
//** This function is called by the main loop and does the actual work of
//** reading the input pins and sending the data via serial

void SampleAndSend()
{   
    x = PIND >> 2; // Shift digital pin port D by 2 to avoid TX and RX pins 
    //(more info: http://www.arduino.cc/en/Reference/PortManipulation)
    Serial.write(x);
    y = PINB; // Read remaining digital pins
    Serial.write(y);
  
for(analogPin=0; analogPin<6; analogPin++) {
  if(setPins[analogPin]==1)
      {
        analogData = analogRead(analogPin); // Read current analog pin value
        Serial.write(analogData >> 7); // shift high bits into output byte
        Serial.write(analogData % 128); // mod by 128 for the small byte
        }
  else
      {
        Serial.write((byte)0x00); // Send 0 to output
        Serial.write((byte)0x00); // 
      }
}     
        
Serial.write(255); // end of packet signifier

/* This bit of code was used for checking how long a full read takes
Serial.println(millis());
 */       
} 

/* Max/MSP patch to receive data (copy and paste in Max/MSP patcher)

----------begin_max5_patcher----------
6004.3oc6c08iiiib+4d.t+GT5GBtO50geSoCXAxgjff8gCCvc3d5RPCYa0d
zNtkLjkmct8P9eOTjR1RtskJolTV1s2AqaaIawh0G+XwhEK9O+Me5gGmm98n
sO58G896dO7v+TckGzWq3JOTcgGd70vuuXc3V8W7wWi1tMbUziOUdy7numqu
Q5lnj8W8kzj7sw+ZTwcvnYnpqmtKecTd9+XSjoUe7Qu+25+ljvW024w+TVb3
58OtjcuFmn9gZRfTc03klFd9O+CLz9u6lv7EeINY0yYQKxMsBEwmQ3O4QnLE
o3QoEuRTj0gFea9+Xsokq2nFxU2pX8U++9Mep3up+7DXNl4gb59B9D8EJts9
hrnODf78wO4wX7hdBNn50dze1KRVj95qQIFY3+Y7p37v0dXx9eUbxxHc2CMX
FPUK7FUleJYad1tE4woIa+i+OI+kcIdwIaiWF4sc2bceOJyKMyKbt56Ep+dy
p9Zdaxh1pdpgEW060zkGTIWGmDsHcmoOQOoJo+LIEyXRvBEleaBkfYRk5kbl
uP8eRkFFWqhIw8SjLXN77c44oGr9diU17vjU0rzluZQ55zLyMQyHAHbf3I06
nn.eDy7N0kvEuqVOnStjnldypjTECZc7hu13aeBtmPaURBzJyDV0q8w3b+Su
lRQiaLWoT70F8ZoOSoBTzCwLtpeq6qDJmDnrkp250el0nZtlPk9GSz12LYY7
1EoIJc57nkdK2E4km5ono3s4JaiBKg3DL8zZ+DvhNdqHnRU+TogyPBMTCGow
QYij5s54LOJ6zCtPZavkmdipOzAYNEKByFh1MSyxJgpMLNBoWLNRmZ2aByT8
o7nrmiRBmadZnNze0CK6go9GSWCWLE8KJ1za0e+C8WxEWn9ZEOCvxVAtIZgC
R+GNoPGumBGr0YW+dEqw+hxx76lkQM.ezfAnOic.DYM.vywwdWl9jFsYsARv
Zqn5uTaji2x839RyPErJDzd6IpCbDKD4TtGyRLOIWOJD1WqBR3Cg40MV5ogK
ITyHehZi2idWvkZd1dxLLII8.073iPlwTCFO5DL9VMjIBtlahkUcl9OonSMI
hOm7u84WdwlSf3ry4DewlvIfQUHHsBJ0LpxXMayyxqdQIH71jloZfyXqS7YH
I04btVYcXhVaDKoEbsRFI0xSF3Ll39BcTBXnhlVvOzzVeH9sQq8PdCP6U6M6
SG96.jIzSMuMFDfBgdH+.+AnOSsu97h0oaitX1+bA.NVU7lXiZ7lNmR25zvk
F8lAo0Ym4PQ5F2DWNUIxnx0NquQ+2Q4dqUy01K8EuvuEFutXBVFfTuPMj5lc
qCyi79ape+tNlFdcVdfcbth62dnOQ0BtR0X8xKsCmKq4PsS7Wu1bXdu9bZrd
kng6v9f84zOXFFKP9A6wRHAVvyyVDKXGKVD1RrPH0DKD+QUrvkyHL7XJVPNV
rXqYnI740EKAipXQMgPpXLsVBbrTgZKoRCLrw0XAq7WzGQ40kJ1X1y8cU0vn
Vc8Ppb.RRoRA2kKqF1lyJt+bfV8xW3KmQILIR5RN.5xxAZctmBAcV.ly88cH
GHvlLf7zUqVG8HvXBu3KQK9ZzxFqBlHPYbFXV6KIiiq+tFvJMVyv1ve5dQB4
CYYTjpAcKrMwz2ezp1kbJVA5oluTGR8DKuxGLglXPBs5IqfMjbmczGqHR6xA
i.0iPgNHT3CDL53dyGLEhfAsT+pAYHRkiH7aBEBpZPB0HlZea9nqPHQCBgPw
1D9XeB6lPgPPlw4j.4cEBkBAdPHDJmv7ETASdSnPfTdsvvjhDM5tBw.yNLzL
FmoyKqqeEBru5QP4ThkUH56TgXsOSnh0.CI3TWljo9WxoBxZc8aDEwIOPpLb
cX+WdQ6+715+7.pZTYeJm4v9u3h1+aMVHbIR4lZ.gJbX+meQ6+zV6+LwLeIB
KcYjPXWz9eqQCjSnyXBLxkhe5Es625dLfaVzPm00IWxtNMnstNiZxHz.K20+
SIgqSWcYs4osuRwlzv2U87Kp0Ns0jamDHbYO+hZnSZUamXVKHW0ycoc94xWx
tWD.JfDB2QLjK6pezJlOVfbYOGOJanrPtaWQVJ0ZoufYizTlIyhQccxkLamJ
ymWfvbr.wZItPod+EI0xIin.ww6SBJ2VBDg3hIPJx93QSf33DgiZqLtRPQWL
HKLYDEHNNE3nRasUhBvWLKjQbHjk9NVdfskAB4BlkUJDKpCxxpyKUjNd+1EX
WqjJox3BawDyjGknntUpHbrTwVXWRFq11fjHG2ASPyvR+QzVwwyJgXKetj3F
RkQ0VoXEGNJU2cqPwwyLgXsj2sb78KgPQnLL4i4nJtdWbasw5ME3kKB9Uw5g
GLlCp35spSf0b.STWnPF0AUnyBH1eLkqx77.iGVhdTrn6LNSdSjrn3hpIBIf
Qum4OEZDCJmu0og.UP4zaBMhhonSnb1cMhBMhAkcf5DS.6ikhaAMhh4jhwRI
4tBgRgXP4OrNScHRRvMQBkGHmwU51h6JDEaVqAsCCJRcGJWpLqtETHXJmHza
4j6JDJEB+AoPTlLS2.dTpIcKpFb1o7UknV+TxlcmudAYmo+g3VNRulZbgOZT
CThYkXXG2x1WzTlOEihjgZ4UpxTJL8YiujAEbbKaeIyeNMINO8LkoTlcjIAs
m15AlkkyD.Dtb.EOqgGUJc0vM33F19r4+ZT9tMuGl7QU+3yCR2szf2tzvT8I
vlTJaTkFl.gcb65fhize6m71luqV49yEHQAzt1195Zajd7QAcTKolmqXaojK
ahJK3dCnvEamhskn8D902nkXxb.43VIW20nXYcRGPexT.mqwKBWuN8WVlEtp
gswoYCERhed2173WhW7Vyl5LoyMHGRu2d4A6gF7oADr3ng6VGNOZ8acDMNO5
0xSVgG+O97elUzSdp3E0Gj0+Pwzdq8Id8O4W+CMtSPieDowmnUeBipdCt5Mj
VGmtTi.6aFsbHZDT2TqpMICloxLhYj2RXgqUSInYEeG5DOlu5k30q0e4mqTA
ez7aO0WQ+JtREgJE5.Vqd295IOqkFP+JARk2u8GQ+5VgIqLLZSZPbhuwlrzB
npRgkhJBrOjn2VUKb953tL.46bDwV21i6qZTlQRdWEezF.I5CW.AFqKuPEpJ
p4NSNkeyQYkb1JVqp6FuN5aQYaOxNRoxuYSsq+P8eTgD4mMMe0QbQw0hSLWi
d3ZYQeKt5QvOb4vLEuIWwX1kY5te+PIBs3IktLJKYW7gw.L5HUj1AVZwZtZx
kbyFqAiXAlyfEdsNegt35zh4OWGM3A8YJSbxwPIGt+xnWB2sN+4yLvZyuvKg
KhN+O+zZTO73pr3koI5C2lFb+hqW0jUyMG2rOo+JIgaN0OWoLp3Qm6taUc1c
amGlUHbJwHI6uadZ55l26vubczK4k2eSbRxwLz7zMsb2r3Ueose87T0ces0G
u9VaedWh41OqPExeda32NhymqFJuDm3nV36gIwuFlGkGajGDzg6ZFv3KaWjk
tdcy9s4Ve6T2ZoREeQzuDuL+KFrs5J.peP7l2rKRT2IdUz17itXd3psGco2B
Hnt19CulmUdAron3fdz2nw4tTCC25nqMuQqG4KPpaqmGg7My7Onwy8THklnP
QEm4Ha4b7ly4B4CGFqoBKYvblli+bZ2x2pzuTxlZUjs1bPuqgj5XXodv3Ycw
3klnagMaYML9stB0CVe8aAd7pKq7BMsjW7tjWkkdQrI8SwkAg9Ci7JXZItDc
It1GoRy34H4GKwk+zRbI6TbYxxdboKl2Bhql6pydvr5bHaSsnGwNyxfAmSgZ
xoNrWOa7a1uOOs+H7MN4T.oNdxSJkyVa56fERd2bP7klC1GPA4jBTf1kPhI3
08Ph7ACBWLojVc5Oa4QuVk+QezjV7IkzpSuYoly6sJ2i9nIsXSJoUmNyRjh5
dG8QSZQmTRKY2gTwrLlAxOjRKxjRZ42kzp7HvEKweHkVSqvl0Y3JMG5lk4ez
GNg0DKlYHXSyBS9X3R3tjMgK9p21S+u2gn6od7+NPLCLfDlrLP99BjMY5Kly
TzaTizAZfhzgKpHmSTALvGbsnJ38DB6Ihf5n7Y6n73LLIbUjx1Kpfc5Uvc8V
FlGNy6+JbwWzhPyE+s613kmVn84Em3k+k3sdKB2F867T3uqCWDUb9.5oQf02
V8TxR27CKS+kDuhrep3.Er3paByTzhmls68a0INTwO3vWVceOEabdTVzROj2
On7n+2MqAs23vFjAU+xJl4z2vD6alOb1IaVVMBJKxLb+pjxXHgr7LpP0Vs9B
d34VSVM0U7ENix01zcYKprRK8Hw6H5bYz173j8IwveeeXaO9K5.yhdS+9W4z
u7Jm9EW4zO+Jm9YW4zO8Jm9IfoexzrCfgOBPoHvkDSegCcIsHlPzBeBQKrID
sP5qx6jy7qftvP5AAtmaVPKDHzh+3PKTHzhbbnEFDZQLNzBGBsvGGZQ.gVXi
CsHgPKzwgV7AgJMRJuAfHlQR6ECCvdjzewf.ewijFLLVCx8DC5B5Wd0MNrQM
drHw7W9rYuH7bXddV77c4lnfzXOnzy7je05z4gqKyA98QXp8jl+Pt0umJG9N
PRGrrArsKe53bl+csYipUom6Q8pnbS3Ud36XVlwdtI7HNYS3QJSrJp+wD1Uu
fZPGtwTr+DUPYBb5snfZPkQNBGMMET3xct9MnfhMDAUYoreBJnLKO4snfhNH
AkAgY5InDXqKmN6tYdooHI0eQXYwMvd+o+pBjSoJP.bl.RKqF0RQwVwuukKB
xs8lc1nvYJBORrXe0n599b999b999b194Ty+xO1RhHFzQxzz.3zZ4RCuyc7f
vbP6gMUIKJQWRSBtB16zSQtc26+Vjwi.S1ibma+t31cu8YK2eH241Vfa24NF
f4emaaMtcmY7OyjHx2411fa2YF6SEx6iRZItsnyLtmhj0NO+tyseWba.I9t+
ccaaws6L20wkE0v6baKvs6b22io241Via2c0fhcmYaKlcmSkDyltiQBnhn7V
1FnhhBqSGi4lEGQVEYviiOnqpJJjQUk8G+QOJYhoyhgFqIyAiGsrzwMQzYGZ
kNh.sWWMv+6Pkruk5nwUmbwWBSVEMXkxysJRiWs5oRRgnkk24gqeRGUN++5z
CL.ZfmMkRaJcRgEzAxKVL0BfAz3NaBZzMBxq.Zu9Bf7hui71mpMSkjxTN.up
PdmbfA9.Y1lPZdcg75O0b4EAcMnJQdw2DHucWMMp51W.nWzcn2dUMMpDUWgX
uSN3.JPlsA6kHulvdYSMlMC5JReag8xg1sGer2f6Pu8p9tWIotBgdmbnAPyO
kqQn2IWvcvPSOkaJnWFAZ2d7gd8uC8VWRQgJotBgdmbnALfL6IIz6fAC3PSZ
rwGLPdGLntjR.URcEtvOSsDWfIAxrMK7yDCLvcItPmoXJVDbQxaA787VvGZB
oZNapuQV9rtOVEq51i+.Xh6CfUOES.mwzliul6Ytv6ANffAxsMEfj6otv6ia
Sfle52VYMFEZ2d7Ae42AeqKoXPkTWiydXxAGvAxsuFyaro1xURDP2sJkXu2F
gwkHg1sGerW1cr25RJenRpqPr2IGZP.Pl8jLxMWYIu.EAcqqcSA8RwP61iOz
K8Nz6IK+VcIotBgdmbnAP2HqWiPuSs36PgtOVusPd4.60iOv68co1oq9ucHn
tBwcmbPARX75IIr6fQB7At0xu.4t+cjf5Bp.fBpqvU8Yxk1BHX7ZyZ93DjfK
9g+H6DGGDoYKM0EWtCOGJ.SgTYKTHaRPgzVnP5TfBIAsPgjIAExagBwSBJD2
BEhbHEB9fP08G4PfOD+b+Q7i.7AmG08moYBvmbdzQfy.9j2j59ywOA3ywOh6
UeEfOHlHt+rDS.+fXx8GPfb3NL3dEXNXDOFw8DC7CtzQfyHlPGoqbvHvt+3m
iCF.dDzX.OHo6UXXfMk7uP9eAmBYxKzTU5AIxtPyUoGj3ExKV3THVdglNUOH
Q5EZ9TLviFwbOnKC7nQb26ACC7H.L26aGCrK3L2ONMCrWub2OSIFXudoifNC
XZw4jBErC3B260.ELJC08nLTvnLT2eT9RAqxHFAhA9D8cuWuTvSzm59C4XJX
TFwHXZ2yCbYGRJD3Q6z8CEP.6tEw8nLDvnLB2aKQ.OERh6QYHvmZ8HvY.ixH
FAqI3gSz8PdD3gSz8l1Xvl1hQ3DuGdD7bONCFdD7buoMFroM28AmGCebR2iy
fAiyfGAEXvl17QPAFLszt9a0MNbL19XwYW5xmMGWqOGlmmEOeWtImHpSV88n
Dc05z4gqKOlP2myHsethd33GcOUZ8iu4+vvN1lsyIs7g8C0IqcOzxrKR+GNY
HGzxVme86U7F+KJOyG.OiJMGNeAC3.IGeioiwaieQEr65Xe57aOyV3Y20wNw
dp+j6a3.wccrOc9c2dK7r65Xmn3fbR9Ew+tN1mNeI9nEd1ccrSTXlNccBCcW
G6SmuDI0BOaZoisKYS3hu5Ee9+MLF5St6O8WVgOk9cq90PzaqIpoJevD3gne
yrtv5WW6sJKc2FOIp+RkB1msvFZ0O5.yzNHZOCknAnlSrNmaazZE84Q379y3
lGlrxlLOT2LOR.u197XrXdEbq46xySS1+neMcYTCKnSqV8FKyNM+BdqL5yu7
RqljJtQwwKJyrUvPFEKTu3Mz8O+rnsQI46CQDtN0TFEmOmTibxTR67nrmiRB
maZCzodVM1rzEjKCU7JWiiT.pTFkogHddMZ61vCaUrCLtMYZzwdCHXG84VwB
JK85DjoJ0wlDi4ojjYoJZx6eObQd72hNrOv5InfcXfT.3AFNGFymDLvsQE8P
ue0iKEnB13t7zzMQIdnA6rfsFUG.uTXJTZB7nBttILIpOcDTqCSvMw1CK0cB
lwth0qtBpSrv4JOCK72HYYiKeF7NRE6b+X+75aMQE+oHeDWjtNMy7CPyBvRA
S9jmFJ2rwQq+KdyfOJatRr2CJTqxBWhqdfRBmET7nXReFEW+cMdxE+HRIB09
19sDvlrzMoYU7EzLZfkD+ZlZjgsx6gNAtccBSQhfQlL5DkCUalviA7pv5qUk
Bsjfy3n.+mpoibRMCTaZFyW81m60gXt0olv8KEyF7Kro3C1zV6BIn8o0DzR1
GGAc2t6F.vxkX7ocRX4pInf.CZtQ5Jt4gyMe0la78R0giWV5JkhN19tPxEyS
p9At87gzdAVq4jRHsG2dsGAR6wj1qAwPZPLxhsHBVKRrWKBoAoVq4DAv5f1y
nP.xp3HI8qwK2jplNRooOMPGugpoKXhZ6wepzAGS7AE7YVh7AIfDsR9DlN9C
zxECxW91OgqhDMe+mrC4K.oegsm3lABTRX2FD2IpDxdMHj1S3as1iKd+FPDo
IZEBRCiDhkzx3ueJrndFpCre44cSYDAZ7IdPcp23lsEndFxBHTX1LolmpCx3
dNr5SVgF06fBQWznzdiKoaPYmMH1tMXPmMHxtMnemFxA1sAwcOVhvxsX2fUb
61hrNskvVtE4c2hV1zfzcKZYaCZ2snkMNvc6mnksN.3Yp8FXk1CXeKzbDPNB
Sr2jYHvlawQyH3nM7+nPBxyPBXaPBfbehXO.IBHugn9tjsCiDDtjsCZd.D6M
yVBHyYJ2k8YXj.xkRdPnnDKhpAJFMGs0nsLaGDIPbJNGroPXuQKAIlcJrZOh
8nSX4XPQx5nsAuc4AvHAL1kbAXQWydNUfAMhN1k16.IApKY6vBqFo8XFnSUW
pTGxONJX+mrifBTbpN1O+iHQgXFm5yDj8zozbXMaa5ryoSQrme+lVjBxSbrE
awtGhRX49HFjauVrO1Y3THL61EC.4goE6gcFCNBwt8PIHm4rXOryn2PP1sGx
A42jE6gcG6FKC1PAM.sk5gvBFuEgZf0hLtEaQXqDsM6ifliAiZwVDzTJXVDO
EAxcVlEw2Pf7dkgsXKBx3fa2FDOlMHnUmTZ01q6kzf3xoC.Ktw.mMPYdGEtY
y2hx1V9PLM9iuF9ylLrR9j4ywIlOa1KCOlE8s3pehIGCeLLawWhyiVjuKyjy
XeWTtAa04fUVxt3J9flFTu7+CgruHw.
-----------end_max5_patcher-----------

*/




