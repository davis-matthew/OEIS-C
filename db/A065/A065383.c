/*
 * A065383  a(n) = smallest prime >= n*(n + 1)/2.
 * https://oeis.org/A065383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime >= n*(n+1)/2
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Compute the triangular number t = n*(n+1)/2 using seq_t. */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Find the first prime not smaller than t. */
    seq_t p = t;
    while (!isPrimeGeneric(p))
        ++p;

    return p;
}
