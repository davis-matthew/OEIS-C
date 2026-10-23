/*
 * A120476  Triangle read by rows: a(n,m)=(2*n-1)*(n-m)*(n+m+1)/2, where n is
 *          the column and m the row index.
 * https://oeis.org/A120476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the (2*n-1)*(n-m)*(n+m+1)/2 entry of A120476, read by
 *            rows (row index m, column index n)
 * Range:     i8 n <= 30, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   isqrt_u64_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64_custom.h"

/* Convert the linear index n into the triangular coordinates (col,row)
   and evaluate a(col,row).                                   */
seq_t a(idx_t n)
{
    /* Compute the row (column index m) that contains element n.   */
    idx_t s = isqrt_u64_custom(8ULL * n + 1ULL);   /* floor(sqrt(8*n+1)) */
    idx_t m = (s + 1ULL) / 2ULL;                  /* column index, m >= 1 */

    /* Number of elements before the current row.             */
    idx_t prev = ((m - 1ULL) * m) / 2ULL;

    /* Position within the row.                               */
    idx_t k = n - prev;                          /* 0 <= k < m */

    /* Evaluate the formula.  All arithmetic is performed in seq_t. */
    seq_t col = (seq_t)k;   /* n in the formula */
    seq_t row = (seq_t)m;   /* m in the formula */

    seq_t val = ( (seq_t)(2 * col - 1) ) * (col - row) * (col + row + 1) / 2;

    return val;
}
