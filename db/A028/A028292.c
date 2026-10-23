/*
 * A028292  Numbers of letters in n (in the Norwegian language Nynorsk).
 * https://oeis.org/A028292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters of n written in Norwegian Nynorsk
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Lengths of the orthographic representations of the numbers 1–19 and of the
   multiples of ten 20–90 in Nynorsk. */
static const unsigned char _unit_len[20] = {
    0, /* unused */
    3, /* 1 ein        */
    2, /* 2 to         */
    3, /* 3 tre        */
    4, /* 4 fire       */
    3, /* 5 fem        */
    4, /* 6 seis       */
    3, /* 7 sju        */
    4, /* 8 åtte       */
    2, /* 9 ni         */
    2, /*10 ti         */
    6, /*11 elleve     */
    4, /*12 tolv       */
    7, /*13 tretten    */
    7, /*14 fjorten    */
    6, /*15 femten     */
    7, /*16 seksten    */
    6, /*17 sytten     */
    5, /*18 atten      */
    6  /*19 nitten     */
};

static const unsigned char _tens_len[10] = {
    0, 0, 4, 6, 5, 5, 6, 5, 4, 5
    /* 0: unused, 10: unused, 20: tjue(4), 30: tretti(6), 40: førti(5),
       50: femti(5), 60: seksti(6), 70: sytti(5), 80: åtte(4), 90: nitti(5) */
};

/* Recursive routine that returns the number of letters of n (in Nynorsk). */
static seq_t _word_len(idx_t n)
{
    if (n < 20) {
        return (seq_t)_unit_len[(int)n];
    }
    if (n < 100) {
        idx_t tens = n / 10;
        idx_t units = n % 10;
        seq_t len = (seq_t)_tens_len[(int)tens];
        if (units) {
            len += (seq_t)_unit_len[(int)units];
        }
        return len;
    }
    if (n < 1000) {
        idx_t hundreds = n / 100;
        idx_t rem = n % 100;
        seq_t len;
        if (hundreds == 1) {
            len = 6; /* "hundre" */
        } else {
            len = (seq_t)_unit_len[(int)hundreds] + 6; /* digit + "hundre" */
        }
        if (rem) {
            len += 2;                   /* "og" */
            len += _word_len(rem);      /* remainder */
        }
        return len;
    }

    /* n >= 1000 : thousands part */
    idx_t thousands = n / 1000;
    idx_t rem = n % 1000;
    seq_t prefix;
    if (thousands == 1) {
        prefix = 5; /* "tusen" */
    } else {
        prefix = _word_len(thousands) + 5; /* thousands word + "tusen" */
    }
    if (rem) {
        return prefix + 2 + _word_len(rem); /* "og" + remainder */
    } else {
        return prefix;
    }
}

seq_t a(idx_t n)
{
    return _word_len(n);
}
