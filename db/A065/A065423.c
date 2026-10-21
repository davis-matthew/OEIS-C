/*
 * A065423  Number of ordered length 2 compositions of n with at least one
 *          even summand.
 * https://oeis.org/A065423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/2) + (n is odd)*floor((n-1)/2)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ordered length‑2 compositions of n with at least one even part. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the number of compositions is 0. */
    if (n <= 1)
        return 0;

    /* t = floor((n-1)/2) */
    seq_t t = ((seq_t)n - 1) / 2;

    /* If n is odd, double t, otherwise return t. */
    return (n & 1) ? (t + t) : t;
}
