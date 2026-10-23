/*
 * A183156  The number T(n) of isometries of all subspaces of the finite
 *          metric space {1,2,...,n} (as a subspace of the reals with the
 *          Euclidean metric).
 * https://oeis.org/A183156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n+1) - (n+2)^2 - 1, n >= 0
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) */
    seq_t pow2 = ((seq_t)1) << (n + 1);

    /* (n+2)^2 */
    seq_t np2 = ((seq_t)n) + 2;
    seq_t np2sq = np2 * np2;

    /* 3*2^(n+1) - (n+2)^2 - 1 */
    seq_t result = (seq_t)3 * pow2 - np2sq - (seq_t)1;

    return result;
}
