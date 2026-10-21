/*
 * A075008  Floor[ concatenation of 7 numbers from n+6 to n in that order
 *          divided by 7].
 * https://oeis.org/A075008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n+6, n+5, … , n) / 7 ) where concat(...) is
 *            the decimal concatenation of the 7 integers in the given order
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 122, u128 n <= 1000,
 *            big any n
 * Helpers:   write_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs the division without ever
   constructing the full concatenated number, so it works
   even when that number does not fit in the native uint64_t. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/write_digits.h"

seq_t a(idx_t n)
{
    /* 7 numbers: n+6 down to n */
    char digit_buf[200];        /* sufficient for all digits (max ~35) */
    int pos = 0;                /* position in digit_buf */

    for (idx_t k = 0; k < 7; ++k) {
        uint64_t num = (uint64_t)(n + 6 - k);   /* safe unsigned arithmetic */
        char temp[25];
        int len = write_digits(num, temp);
        for (int i = 0; i < len; ++i)
            digit_buf[pos++] = temp[i];
    }

    /* Long division by 7 while constructing the quotient digit by digit. */
    int remainder = 0;       /* remainder < 7, fits in int */
    seq_t quotient = 0;      /* result */

    for (int i = 0; i < pos; ++i) {
        int d = digit_buf[i] - '0';
        remainder = remainder * 10 + d;
        int qdigit = remainder / 7;
        remainder = remainder % 7;
        quotient = quotient * 10 + qdigit;
    }

    /* quotient is floor(concatenation / 7) */
    return quotient;
}
