/*
 * A061709  Consider a (hollow) triangle with n cells on each edge, for a
 *          total of 3(n-1) cells if n>1, or 1 cell if n=1; a(n) is number of
 *          ways of labeling cells with 0's and 1's; triangle may be rotated
 *          and turned over.
 * https://oeis.org/A061709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2^(3*(n-1)) + 2^n + 3*2^floor((3*n-1)/2)) / 6 for n>1,
 *            a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* If seq_t is 8 bytes or less we use 128‑bit intermediates.
       Otherwise we rely on big_pow for arbitrary precision. */
    if (sizeof(seq_t) <= 8) {
        /* cast intermediate to __uint128_t to avoid overflow */
        __uint128_t power1 = (__uint128_t)1;
        idx_t exp1 = 3 * (n - 1);
        power1 <<= exp1;                     /* 2 ^ (3*(n-1)) */

        __uint128_t power2 = (__uint128_t)1 << n;   /* 2 ^ n */

        idx_t exp3 = (3 * n - 1) / 2;
        __uint128_t power3 = (__uint128_t)1 << exp3;   /* 2 ^ floor((3*n-1)/2) */
        power3 *= 3;                          /* 3 * 2 ^ ... */

        __uint128_t sum = power1 + power2 + power3;
        __uint128_t result = sum / 6;

        return (seq_t)result;
    } else {
        /* seq_t is arbitrary‑precision, big_pow returns a full value. */
        seq_t pow1 = big_pow((seq_t)2, 3 * (n - 1));
        seq_t pow2 = big_pow((seq_t)2, n);
        seq_t pow3 = big_pow((seq_t)2, (3 * n - 1) / 2);

        seq_t sum = pow1 + pow2 + 3 * pow3;
        return sum / 6;
    }
}
