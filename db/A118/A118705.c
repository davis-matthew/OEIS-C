/*
 * A118705  a(n) = determinant of n X n circulant matrix whose first row is
 *          the first n triangular numbers A000217(0), A000217(1), ...
 *          A000217(n-1).
 * https://oeis.org/A118705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1)*n^(n-2)*(n+1)*(n-1)*((n+1)^n-(n-1)^n)/(6*2^n)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Helper for small integer types (<=64 bits) – use 128‑bit intermediate */
static inline __int128 pow_int128(__int128 base, idx_t exp)
{
    __int128 result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Branch that keeps 128‑bit intermediates for 64‑bit seq_t */
    if (sizeof(seq_t) <= 8) {
        __int128 N    = (__int128)n;
        __int128 Np1  = N + 1;
        __int128 Nm1  = N - 1;

        __int128 powN   = pow_int128(N, n - 2);      /* n^(n-2)   */
        __int128 prod   = Np1 * Nm1;                  /* (n+1)(n-1)*/
        __int128 powNp1 = pow_int128(Np1, n);        /* (n+1)^n   */
        __int128 powNm1 = pow_int128(Nm1, n);        /* (n-1)^n   */
        __int128 diff   = powNp1 - powNm1;
        __int128 numerator = powN * prod * diff;
        __int128 denom = 6 * (( (__int128)1 ) << n); /* 6*2^n      */

        __int128 result = numerator / denom;
        if ( (n & 1) == 0 )
            result = -result;

        return (seq_t)result;
    } else {
        /* Arbitrary‑precision branch – use seq_pow from utils */
        seq_t N    = (seq_t)n;
        seq_t Np1  = N + 1;
        seq_t Nm1  = N - 1;

        seq_t powN   = seq_pow(N, n - 2);           /* n^(n-2)   */
        seq_t prod   = Np1 * Nm1;                   /* (n+1)(n-1)*/
        seq_t powNp1 = seq_pow(Np1, n);             /* (n+1)^n   */
        seq_t powNm1 = seq_pow(Nm1, n);             /* (n-1)^n   */
        seq_t diff   = powNp1 - powNm1;
        seq_t numerator = powN * prod * diff;
        seq_t denom = 6 * seq_pow((seq_t)2, n);      /* 6*2^n     */
        seq_t result = numerator / denom;
        if ( (n & 1) == 0 )
            result = -result;

        return result;
    }
}
