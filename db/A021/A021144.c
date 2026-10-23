/*
 * A021144  Expansion of 1/((1-x)*(1-2*x)*(1-5*x)*(1-10*x)).
 * https://oeis.org/A021144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 97*a(n-2) + 180*a(n-3) - 100*a(n-4) a(0) = 1,
 *            a(1) = 18, a(2) = 227, a(3) = 2520 Computes the n-th term of the
 *            expansion of 1/((1-x)*(1-2x)*(1-5x)*(1-10x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)227;
    if (n == 3) return (seq_t)2520;

    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)18;     /* a(n-3) */
    seq_t a2 = (seq_t)227;    /* a(n-2) */
    seq_t a3 = (seq_t)2520;   /* a(n-1) */
    seq_t an;                 /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)18 * a3
            - (seq_t)97 * a2
            + (seq_t)180 * a1
            - (seq_t)100 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
