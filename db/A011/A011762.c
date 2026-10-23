/*
 * A011762  Number of letters in n (in Spanish).
 * https://oeis.org/A011762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = length of the Spanish word for n, ignoring spaces and
 *            accents. The sequence a(0)=4 ("cero"), a(1)=3 ("uno") etc
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t units_len[10] = {
    0,  /* unused index 0 */
    3,  /* 1  "uno"    */
    3,  /* 2  "dos"    */
    4,  /* 3  "tres"   */
    6,  /* 4  "cuatro" */
    5,  /* 5  "cinco"  */
    4,  /* 6  "seis"   */
    5,  /* 7  "siete"  */
    4,  /* 8  "ocho"   */
    5   /* 9  "nueve"  */
};

static const seq_t len_1_19[20] = {
    4, /* 0  "cero"   */
    3, /* 1  "uno"    */
    3, /* 2  "dos"    */
    4, /* 3  "tres"   */
    6, /* 4  "cuatro" */
    5, /* 5  "cinco"  */
    4, /* 6  "seis"   */
    5, /* 7  "siete"  */
    4, /* 8  "ocho"   */
    5, /* 9  "nueve"  */
    4, /*10  "diez"   */
    4, /*11  "once"   */
    4, /*12  "doce"   */
    5, /*13  "trece"  */
    7, /*14  "catorce"*/
    6, /*15  "quince" */
    9, /*16  "dieciséis" -> "dieciseis" */
   10, /*17  "diecisiete" */
    9, /*18  "dieciocho" */
   10  /*19  "diecinueve"*/
};

static const seq_t tens_len[10] = {
    0, /* unused indices 0,1,2 */
    0,  /* 1,2 */
    0,  /* 1,2 */
    7,  /* 3  "treinta"  */
    8,  /* 4  "cuarenta" */
    9,  /* 5  "cincuenta"*/
    7,  /* 6  "sesenta"  */
    7,  /* 7  "setenta"  */
    7,  /* 8  "ochenta"  */
    7   /* 9  "noventa"  */
};

static const seq_t hundreds_len[10] = {
    0,  /* unused 0 */
    0,  /* unused 1 */
    10, /* 2  "doscientos" */
    11, /* 3  "trescientos" */
    13, /* 4  "cuatrocientos" */
    10, /* 5  "quinientos"   */
    11, /* 6  "seiscientos"  */
    11, /* 7  "setecientos"  */
    11, /* 8  "ochocientos"  */
    11  /* 9  "novecientos"  */
};

static const seq_t sing_len[12] = {
    0, /* 0 unused */
    0, /* 1 unused */
    6, /* 2 "millón"   */
    6, /* 3 "billón"   */
    7, /* 4 "trillón"  */
    9, /* 5 "cuatrillón" */
    9, /* 6 "quintillón" */
    9, /* 7 "sextillón"  */
    9, /* 8 "septillón"  */
    8, /* 9 "octillón"   */
    8, /*10 "nonillón"   */
    8  /*11 "decillón"   */
};

static seq_t count10_99(uint64_t n)
{
    if (n < 20)
        return len_1_19[n];
    if (n == 20)
        return 6;                 /* "veinte" */
    if (n < 30) {
        uint64_t u = n - 20;      /* 1–9 */
        return 6 + units_len[u]; /* "veinti" + unit */
    }
    uint64_t t = (n / 10) * 10;  /* 30,40,…,90 */
    uint64_t u = n % 10;
    if (u == 0)
        return tens_len[t / 10];
    return tens_len[t / 10] + 1 + units_len[u]; /* tens + "y" + unit */
}

static seq_t countHundreds(uint64_t n)
{
    if (n == 0)
        return 0;
    if (n == 100)
        return 4; /* "cien" */
    uint64_t h = n / 100;
    uint64_t r = n % 100;
    seq_t len_h;
    if (h == 1)
        len_h = 6;              /* "ciento" */
    else
        len_h = hundreds_len[h];
    if (r == 0)
        return len_h;
    return len_h + count10_99(r);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4; /* "cero" */
    seq_t total = 0;
    idx_t rem = n;
    int group = 0; /* 0 units, 1 thousands, 2 millions, … */
    while (rem > 0) {
        uint64_t g = (uint64_t)(rem % 1000);
        if (g != 0) {
            if (group == 1) { /* thousands */
                total += (g == 1) ? (seq_t)3
                                  : (countHundreds(g) + (seq_t)3);
            } else if (group >= 2) { /* millions, billions, … */
                if (g == 1) {
                    total += (seq_t)(2 + sing_len[group]); /* "un <group>" */
                } else {
                    total += countHundreds(g) + (seq_t)(sing_len[group] + 2);
                }
            } else { /* group == 0 */
                total += countHundreds(g);
            }
        }
        rem /= 1000;
        ++group;
    }
    return total;
}
