/*
 * A174960  Smallest prime p such that p + n*(n+1)/2 is prime, or 0 if no such
 *          prime exists.
 * https://oeis.org/A174960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p + n*(n+1)/2 is prime, or 0
 *            if no such prime exists
 * Range:     u8 n <= 22, u16 n <= 1242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Compute triangular number T = n*(n+1)/2 in seq_t */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* If T is odd, only p = 2 can make p + T odd; test it */
    if ((T & 1) != 0) {
        return isPrimeGeneric(2 + T) ? (seq_t)2 : (seq_t)0;
    }

    /* Even triangular number: search for the smallest prime p with p + T prime */
    for (seq_t p = 2; ; ++p) {
        if (!isPrimeGeneric(p))
            continue;
        if (isPrimeGeneric(p + T))
            return p;
    }
}
