/*
 * A181534  Minimum number of rounds to be played to decide a two-player game
 *          of up to n rounds in which the winner of round j receives j
 *          points.
 * https://oeis.org/A181534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = smallest k such that k*(k+1) > n*(n+1)/2
 * Range:     u8 n <= 5, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9107 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;

    seq_t nseq = (seq_t)n;

    /* Compute discriminant: 1 + 4 * T(n) = 1 + 2*n*(n+1) */
    seq_t disc = 1 + 2 * nseq * (nseq + 1);

    /* Floor of square root of the discriminant */
    seq_t s = floor_sqrt(disc);

    /* Largest k with k*(k+1) <= T(n) */
    seq_t k0 = (s - 1) / 2;

    /* Smallest k with k*(k+1) > T(n) */
    return k0 + 1;
}
