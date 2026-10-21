/*
 * A056597  Number of letters in n in Serbian or Croatian.
 * https://oeis.org/A056597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of letters in the Serbian/Croatian spelling of n (n ≥ 1) */

#include <stdint.h>

/* Length of the words for the digits 0–9 (0 unused) */
static const uint8_t UNIT_LEN[10] = {
    0, /* 0 is unused */
    5, /* jedan   */
    3, /* dva     */
    3, /* tri     */
    6, /* četiri  */
    3, /* pet     */
    4, /* šest    */
    5, /* sedam   */
    4, /* osam    */
    5  /* devet   */
};

/* Length of the words for the numbers 10–19 */
static const uint8_t TEEN_LEN[10] = {
    5, /* deset       */
    9, /* jedanaest   */
    8, /* dvanaest    */
    8, /* trinaest    */
    9, /* četrnaest   */
    8, /* petnaest    */
    8, /* šesnaest    */
    10,/* sedamnaest  */
    9, /* osamnaest   */
    10 /* devetnaest  */
};

/* Length of the words for the multiples of ten (20…90) */
static const uint8_t TENS_LEN[10] = {
    0, /* 0 */
    0, /* 1 */
    8, /* 20   */
    8, /* 30   */
    9, /* 40   */
    7, /* 50   */
    8, /* 60   */
    10,/* 70   */
    9, /* 80   */
    9  /* 90   */
};

/* Length of the words for the hundreds 100…900 */
static const uint8_t HUNDRED_LEN[10] = {
    0, /* 0 */
    3, /* sto */
    5, /* dvesta */
    5, /* trista */
    9, /* četiristo */
    5, /* petsto */
    7, /* šeststo */
    7, /* sedamsto */
    6, /* osamsto */
    8  /* devetsto */
};

/* Recursive helper that returns the number of letters for n (n ≥ 1) */
static seq_t count_letters(idx_t n)
{
    /* 1–9 */
    if (n < 10) {
        return (seq_t)UNIT_LEN[(uint8_t)n];
    }

    /* 10–19 */
    if (n < 20) {
        return (seq_t)TEEN_LEN[(uint8_t)(n - 10)];
    }

    /* 20–99 */
    if (n < 100) {
        uint8_t tens   = (uint8_t)(n / 10);
        uint8_t units  = (uint8_t)(n % 10);
        seq_t    len   = (seq_t)TENS_LEN[tens];
        if (units != 0) {
            len += (seq_t)UNIT_LEN[units];
        }
        return len;
    }

    /* 100–999 */
    if (n < 1000) {
        uint8_t h   = (uint8_t)(n / 100);
        uint8_t rem = (uint8_t)(n % 100);
        seq_t    len = (seq_t)HUNDRED_LEN[h];
        if (rem != 0) {
            len += count_letters(rem);
        }
        return len;
    }

    /* 1 000–999 999 (thousands) */
    if (n < 1000000) {
        idx_t t   = n / 1000;     /* thousands part */
        idx_t rem = n % 1000;     /* remainder   */
        seq_t len;

        if (t == 1) {
            len = 7;              /* "hiljada"   */
        } else if (t == 2) {
            len = 5 + 7;           /* "dvije hiljade" */
        } else {
            len = count_letters(t) + 7;
        }

        if (rem != 0) {
            len += count_letters(rem);
        }
        return len;
    }

    /* 1 000 000–999 999 999 (millions) */
    if (n < 1000000000) {
        idx_t m   = n / 1000000;   /* millions part */
        idx_t rem = n % 1000000;   /* remainder   */
        seq_t len;

        if (m == 1) {
            len = 6;              /* "milion"    */
        } else {
            len = count_letters(m) + 7; /* plural "miliona" */
        }

        if (rem != 0) {
            len += count_letters(rem);
        }
        return len;
    }

    /* For larger numbers, fallback to handle additional magnitudes
       (optional extension). Here we simply disallow such inputs. */
    return 0;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for the sequence, but safe guard */
    }
    return count_letters(n);
}
