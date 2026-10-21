/*
 * A231471  Largest integer less than 11 and coprime to n.
 * https://oeis.org/A231471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 696, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find largest integer <11 that is coprime to n by brute force searching
 * from 10 down to 1. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Search for the greatest k (1 ≤ k ≤ 10) with gcd(n,k) == 1 */
    for (int k = 10; k >= 1; k--)
    {
        if (gcd((seq_t)n, (seq_t)k) == (seq_t)1)
            return (seq_t)k;
    }
    /* Should never reach here, but return 1 as a fallback */
    return (seq_t)1;
}
