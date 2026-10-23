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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

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
