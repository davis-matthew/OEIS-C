/*
 * A180969  Array read by antidiagonals: a(k,n) = natural numbers each
 *          repeated 2^k times.
 * https://oeis.org/A180969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, less
 * Formula:   a(n) = floor((d - offset) / 2^offset), where d = floor((sqrt(8*n
 *            + 1) - 1) / 2) and offset = n - d*(d+1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute d = floor((sqrt(8*n + 1) - 1)/2) */
    long double val = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t d = (idx_t)((val - 1.0L) / 2.0L);

    /* Compute offset = n - d*(d+1)/2 */
    idx_t tri = d * (d + 1) / 2;
    idx_t offset = n - tri;

    /* k = offset (row index), col = d - offset (column index) */
    idx_t k = offset;
    idx_t col = d - offset;

    /* Convert column to seq_t and divide by 2^k via repeated shifts */
    seq_t val_col = (seq_t)col;
    for (idx_t i = 0; i < k; ++i)
        val_col >>= 1;

    return val_col;
}
