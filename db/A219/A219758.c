/*
 * A219758  Expansion of x^4*(2-6*x+5*x^2+2*x^3-4*x^4)/((1-x)^2*(1-2*x)^5).
 * https://oeis.org/A219758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-7)*(n^4-2*n^3-25*n^2+194*n-576)/3 + n + 1 for n > 1,
 *            a(0) = a(1) = 0 (derived from the OEIS entry for A219758)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 50, u128 n <= 109,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* signed 128‑bit intermediate to prevent overflow of intermediate
       calculations and to correctly handle negative polynomial values
       for small n. */
    __int128 N   = (__int128)n;
    __int128 poly = N * N * N * N;          /* n^4          */
    poly -= (__int128)2 * N * N * N;        /* - 2 n^3      */
    poly -= (__int128)25 * N * N;           /* -25 n^2      */
    poly += (__int128)194 * N;              /* + 194 n      */
    poly -= (__int128)576;                  /* - 576        */

    /* the polynomial is always divisible by 3 for n>1 */
    __int128 tmp = poly / 3;

    if (n > 7) {
        /* For n > 7 we need to multiply by 2^(n-7).
           Use the arbitrary‑precision power function for full correctness. */
        seq_t pow = big_pow((seq_t)2, (seq_t)(n - 7));
        seq_t tmp_big = (seq_t)tmp * pow;
        return tmp_big + (seq_t)N + (seq_t)1;
    } else {
        /* For 2 <= n <= 7 we can divide by 2^(7-n) safely. */
        unsigned shift = (unsigned)(7 - n);          /* 1 .. 5 */
        __int128 divisor = ((__int128)1) << shift;   /* 2^(7-n) */
        tmp /= divisor;                              /* exact integer division */
        return (seq_t)(tmp + N + (seq_t)1);
    }
}
