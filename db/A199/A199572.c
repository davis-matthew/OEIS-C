/*
 * A199572  Number of round trips of length n on the cycle graph C_2 from any
 *          of the two vertices.
 * https://oeis.org/A199572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = (2^n + (-2)^n)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 2^n for even n, 0 for odd n. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (n & 1) ? 0 : pow2(n);
}
