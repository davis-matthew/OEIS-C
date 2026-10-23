/*
 * A134300  Period 8: repeat [6, -4, 2, -1, 2, -4, 6, -7].
 * https://oeis.org/A134300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = period[ n mod 8 ] where period = [6, -4, 2, -1, 2, -4, 6,
 *            -7]
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t period[8] = {
    6,  /* a(0) */
   -4,  /* a(1) */
    2,  /* a(2) */
   -1,  /* a(3) */
    2,  /* a(4) */
   -4,  /* a(5) */
    6,  /* a(6) */
   -7   /* a(7) */
};

seq_t a(idx_t n)
{
    /* Compute n modulo 8 (period length) safely and return the
       corresponding element of the period. */
    return period[(size_t)(n & ((idx_t)7))];
}
