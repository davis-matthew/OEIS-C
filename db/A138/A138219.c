/*
 * A138219  Integers related to the volume k(n) of a unit hypersphere in n
 *          dimensions.
 * https://oeis.org/A138219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is even, and 2^((n+1)/2) if n is odd
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the n-th term of OEIS A138219. */
seq_t a(idx_t n)
{
    /* Even n -> a(n) = n. */
    if ((n & 1ULL) == 0)
        return (seq_t)n;

    /* Odd n -> a(n) = 2^((n+1)/2). */
    idx_t e = (n + 1) >> 1;          /* (n+1)/2 */
    return ((seq_t)1 << e);          /* Shift 1 left by e positions. */
}
