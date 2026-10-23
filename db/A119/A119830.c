/*
 * A119830  Bi-diagonal inverse of (2n)!/(2k)!.
 * https://oeis.org/A119830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   * a(n) = 1 if n+1 is a triangular number * a(n) = -2k(2k-1) if n
 *            = (k^2+3k-2)/2 for some k>=1 * a(n) = 0 otherwise
 * Range:     i8 n <= 25, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 128‑bit integer square root, returns floor(sqrt(x)) for x<=2^128-1.
 *  Uses binary search on 64‑bit values – the result never exceeds 2^64. */
static inline unsigned long long isqrt_u128(__int128 x)
{
    unsigned long long lo = 0, hi = (unsigned long long)1 << 32; /* 2^32 > sqrt(x) for x<2^67 */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Check for a(n) = 1 : n+1 triangular  <=> 1+8*(n+1) is a perfect square */
    unsigned long long m = (unsigned long long)n + 1ULL;
    __int128 disc1 = (__int128)1 + (__int128)8 * m;
    unsigned long long sr1 = isqrt_u128(disc1);
    if ((__int128)sr1 * sr1 == disc1)
        return (seq_t)1;

    /* Check for a(n) = -2k(2k-1) : n = (k^2+3k-2)/2  <=> 8n+17 is a perfect square */
    __int128 disc2 = (__int128)8 * n + (__int128)17;
    unsigned long long sr2 = isqrt_u128(disc2);
    if ((__int128)sr2 * sr2 == disc2) {
        if (sr2 >= 3 && ((sr2 - 3) & 1) == 0) {
            unsigned long long k = (sr2 - 3) / 2;
            if (k >= 1) {
                seq_t kk = (seq_t)k;
                return - (seq_t)2 * kk * ((seq_t)2 * kk - 1);
            }
        }
    }

    return (seq_t)0;
}
