/*
 * A193930  E.g.f.: exp(x+x^2+x^3+x^4).
 * https://oeis.org/A193930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0) = 1
 *           a(1) = 1
 *           a(2) = 3
 *           a(3) = 13
 *           a(n) = a(n-1) + 2*(n-1)*a(n-2) + 3*(n-1)(n-2)*a(n-3)
 *                  + 4*(n-1)(n-2)(n-3)*a(n-4)   for n >= 4
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)13;

    /* Cache the four most recent values to apply the recurrence. */
    seq_t a0 = 1;        /* a(n-4) */
    seq_t a1 = 1;        /* a(n-3) */
    seq_t a2 = 3;        /* a(n-2) */
    seq_t a3 = 13;       /* a(n-1) */
    seq_t an = 0;        /* a(n) */

    /* Iterate from 4 up to n, updating the four recent terms each time. */
    for (idx_t i = 4; i <= n; ++i) {
        an  = a3; /* a(n-1) term */
        an += (seq_t)2 * (seq_t)(i-1) * a2;   /* 2*(n-1)*a(n-2) */
        an += (seq_t)3 * (seq_t)(i-1) * (seq_t)(i-2) * a1; /* 3*(n-1)(n-2)*a(n-3) */
        an += (seq_t)4 * (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3) * a0; /* 4*(n-1)(n-2)(n-3)*a(n-4) */

        /* Shift terms for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
