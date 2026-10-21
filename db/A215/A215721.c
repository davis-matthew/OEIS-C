/*
 * A215721  The values of N for which the 1 X N domino-covering game is a
 *          second player win.
 * https://oeis.org/A215721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 9641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215721: values of N for which the 1 X N domino‑covering game
   is a second player win.
   Initial values:
     a(1)=0, a(2)=1, a(3)=5, a(4)=9, a(5)=15, a(6)=21, a(7)=25,
     a(8)=29, a(9)=35, a(10)=39, a(11)=43, a(12)=55, a(13)=59,
     a(14)=63.
   Recurrence for n>14:
     a(n) = a(n-5) + 34. */
#include <stddef.h>

extern seq_t a(idx_t n)
{
    /* 15 elements: index 0 unused, indices 1..14 hold the first 14 terms. */
    static const seq_t init[15] = {
        0,   /* dummy 0 for 0‑based array */
        0,  /* a(1) */
        1,  /* a(2) */
        5,  /* a(3) */
        9,  /* a(4) */
        15, /* a(5) */
        21, /* a(6) */
        25, /* a(7) */
        29, /* a(8) */
        35, /* a(9) */
        39, /* a(10) */
        43, /* a(11) */
        55, /* a(12) */
        59, /* a(13) */
        63   /* a(14) */
    };

    if (n <= 14)
        return init[(size_t)n];

    /* For n>14 use: a(n)=a(n-5)+34 repeatedly until the base index is
       between 10 and 14 inclusive.  The number of steps of -5 is
       k = (n-15)/5 + 1, and the base index is n-5*k. */
    idx_t k   = (n - 15) / 5 + 1;
    idx_t base = n - 5 * k;                /* 10 <= base <= 14 */
    seq_t inc  = (seq_t)(34 * k);          /* compute increment in idx_t */
    return init[(size_t)base] + inc;
}
