/*
 * A151332  Number of walks within N^2 (the first quadrant of Z^2) starting
 *          and ending at (0,0) and consisting of 4 n steps taken from {(-1,
 *          -1), (-1, 1), (1, 0)}.
 * https://oeis.org/A151332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = A000108(n) * A000108(2*n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute the product of Catalan number C(n) and C(2n). */
    seq_t c1 = catalan(n);
    seq_t c2 = catalan(2 * n);
    return c1 * c2;
}
