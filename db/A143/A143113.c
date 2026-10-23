/*
 * A143113  a(n) = the smallest positive integer that is coprime to n and has
 *          the same number of 1's in its binary representation as n has in
 *          binary.
 * https://oeis.org/A143113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest positive integer that is coprime to n * and
 *            has the same number of 1's in its binary representation as n
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Find the smallest integer coprime to n with popcount equal to that of n. */
static inline seq_t smallest_coprime_same_popcount(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Number of 1-bits in n (popcount). */
    unsigned int bits = __builtin_popcountll((unsigned long long)n);
    /* Smallest integer with 'bits' ones: (1<<bits) - 1. */
    unsigned long long cand = ((unsigned long long)1 << bits) - 1ULL;

    for (;;)
    {
        /* If cand is coprime to n, we have found the answer. */
        if (gcd(sn, (seq_t)cand) == (seq_t)1)
            return (seq_t)cand;

        /* Generate next integer with the same popcount (Gosper's hack). */
        unsigned long long c = cand & -cand;
        unsigned long long r = cand + c;
        cand = (((r ^ cand) >> 2) / c) | r;
        /* The loop will eventually hit a coprime candidate. */
    }
}

seq_t a(idx_t n)
{
    return smallest_coprime_same_popcount(n);
}
