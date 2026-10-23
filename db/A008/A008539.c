/*
 * A008539  Numbers that do not contain the letter `s'.
 * https://oeis.org/A008539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth integer whose English word representation does not contain the letter 's'. */
#include <stdbool.h>

/* Digits and tens that entail the letter 's' in English. */
static const bool unit_has_s[10] = {
    /*0*/ false, /*1*/ false, /*2*/ false, /*3*/ false, /*4*/ false,
    /*5*/ false, /*6*/ true , /*7*/ true , /*8*/ false, /*9*/ false
};
static const bool teen_has_s[10] = {
    /*10*/ false, /*11*/ false, /*12*/ false, /*13*/ false, /*14*/ false,
    /*15*/ false, /*16*/ true , /*17*/ true , /*18*/ false, /*19*/ false
};
static const bool tens_has_s[10] = {
    /*0*/ false, /*1*/ false, /*2*/ false, /*3*/ false, /*4*/ false,
    /*5*/ false, /*6*/ true , /*7*/ true , /*8*/ false, /*9*/ false
};

/* Return true if the English representation of num contains the letter 's'. */
static inline bool number_has_s(seq_t num)
{
    /* zero has no 's' */
    if (num == (seq_t)0)
        return false;

    seq_t remaining = num;
    /* Process each 3‑digit group (units, thousands, millions, …). */
    while (remaining > (seq_t)0) {
        seq_t grp = remaining % (seq_t)1000;   /* 0‑999 */
        remaining    = remaining / (seq_t)1000;

        if (grp != (seq_t)0) {
            /* Hundreds part */
            seq_t hundreds = grp / (seq_t)100;
            if (hundreds != (seq_t)0) {
                uint8_t h = (uint8_t)(hundreds + (seq_t)0); /* small enough */
                if (unit_has_s[h])
                    return true;
            }

            seq_t rem = grp % (seq_t)100;
            if (rem >= (seq_t)10 && rem <= (seq_t)19) {   /* teens */
                uint8_t t = (uint8_t)(rem - (seq_t)10);
                if (teen_has_s[t])
                    return true;
            } else {
                seq_t tens = rem / (seq_t)10;
                seq_t units = rem % (seq_t)10;
                if (tens != (seq_t)0) {
                    uint8_t t = (uint8_t)(tens);
                    if (tens_has_s[t])
                        return true;
                }
                if (units != (seq_t)0) {
                    uint8_t u = (uint8_t)(units);
                    if (unit_has_s[u])
                        return true;
                }
            }
            /* Scale words (“thousand”, “million”, …) contain no ‘s’. */
        }
    }
    return false;
}

/* Return the nth term (1‑based) of OEIS A008539. */
seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   value = (seq_t)0;

    for (;;) {
        if (!number_has_s(value)) {
            ++count;
            if (count == n)
                return value;
        }
        ++value;               /* seq_t supports ++ operator */
    }
}
