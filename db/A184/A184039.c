/*
 * A184039  T(n,k) = 1/16 the number of (n+1) X (k+1) 0..3 arrays with all 2 X
 *          2 subblocks having the same four values.
 * https://oeis.org/A184039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the coordinates of the nth element
 *            enumerated along anti‑diagonals of the table defined by T(n,k) =
 *            -14 + 9*(2^(n-1)+2^(k-1)) +
 *            3*(2^((n-1)/2)+2^(n/2)+2^((k-1)/2)+2^(k/2)) The function returns
 *            a(n) for any n >= 1
 * Range:     u8 n <= 15, u16 n <= 91, u32 n <= 287, u64 n <= 287,
 *            u128 n <= 287, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal coordinates (i,j) corresponding to index n */
    idx_t d   = 1;      /* diagonal number (sum of indices)   */
    idx_t sum = 1;      /* total elements up to current diag */

    while (sum < n) {    /* locate the smallest diagonal containing n */
        ++d;
        sum += d;
    }

    idx_t prev    = sum - d;    /* elements in all previous diagonals */
    idx_t offset  = n - prev;   /* position within current diagonal (1‑based) */
    idx_t i = offset;           /* row index (>=1) */
    idx_t j = d - offset + 1;   /* column index (>=1) */

    /* Compute T(i,j) using the given closed form */
    seq_t value = 0;
    value += (seq_t)9 * (pow2(i - 1) + pow2(j - 1));
    value += (seq_t)3 * (pow2((i - 1) / 2) + pow2(i / 2) +
                         pow2((j - 1) / 2) + pow2(j / 2));
    value -= (seq_t)14;

    return value;
}
