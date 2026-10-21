/*
 * A095694  T(n,3) diagonal of triangle in A095693.
 * https://oeis.org/A095694
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1)*n*(n - 1)*(n^3 + 3*n^2 + 2*n - 16)/48
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2 (a(2)=1) */

#include <stdint.h>

/* Compute the n-th term of OEIS A095694 (diagonal of triangle A095693). */
seq_t a(idx_t n)
{
    /*  The sequence is defined only for n >= 2.  Values for smaller n are
     *  arbitrary (0 in this implementation). */
    if (n < 2)
        return (seq_t)0;

    /* Use 128‑bit arithmetic to avoid intermediate overflow. */
    __uint128_t N = n;
    __uint128_t A = N + 1;                /* (n + 1) */
    __uint128_t B = N;                    /* n */
    __uint128_t C = N - 1;                /* (n - 1) */
    __uint128_t inner = N * N * N         /* n^3 */
                     + (__uint128_t)3 * N * N   /* 3*n^2 */
                     + (__uint128_t)2 * N       /* 2*n */
                     - 16;                      /* -16 */

    __uint128_t prod = A * B;             /* (n + 1)*n */
    prod *= C;                            /* * (n - 1) */
    prod *= inner;                        /* * inner term */
    prod /= 48;                           /* divide by 48 */

    return (seq_t)prod;
}
