/*
 * A134310  (A000012 * A134309 + A134309 * A000012) - A000012, where the
 *          sequences are interpreted as lower triangular matrices.
 * https://oeis.org/A134310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = d_i + d_j - 1 where d_k = 1 if k <= 1 d_k = 2^(k-1)
 *            otherwise (i, j) are the row and column of n in the
 *            lower‑triangular array with rows numbered from 0, columns from
 *            0. The array entries are defined by a[i][j] = d_j + d_i - 1 for
 *            i >= j, 0 otherwise
 * Range:     u8 n <= 44, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Determine the row i and column j of the flattened index n. */
    idx_t i = 0;
    /* find smallest i such that (i+1)(i+2)/2 > n */
    while ( ((i + 1) * (i + 2)) / 2 <= n )
        i++;

    /* number of entries in all preceding rows */
    idx_t prev = (i * (i + 1)) / 2;
    idx_t j = n - prev;   /* column index within row i */

    /* diagonal values d_i and d_j */
    seq_t di = (i == 0) ? (seq_t)1 : pow2(i - 1);
    seq_t dj = (j == 0) ? (seq_t)1 : pow2(j - 1);

    /* a(n) = d_i + d_j - 1 */
    return di + dj - (seq_t)1;
}
