/*
 * A194011  Natural interspersion of A002061; a rectangular array, by
 *          antidiagonals.
 * https://oeis.org/A194011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   array_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n)-th element of the array whose rows are
 * the natural interspersion of A002061, taken by antidiagonals.
 * The row r and column c (both 1‑based) entry is
 *
 *   T(r,c) = c^2 - c + 1
 *            + floor(r/2)
 *            + (sum of odd integers from 3 to the largest odd ≤ r)
 *            + 2(c-1) * (# of odd integers from 3 to the largest odd ≤ r)
 *
 * where the summation term and the count of odd integers are zero
 * when r < 3.  This expression follows directly from the recurrence
 * defining each row.
 */

#include <stdint.h>
#include "../utils/array_row_col.h"

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++d;
        cumulative += d;          /* cumulative = d*(d+1)/2 */
    }

    /* Index within the d-th antidiagonal (1‑based). */
    idx_t prev_cumulative = cumulative - d;   /* (d-1)*d/2 */
    idx_t pos_in_diag   = n - prev_cumulative; /* 1 … d */

    /* r increases from 1 to d along the diagonal. */
    idx_t r = pos_in_diag;
    idx_t c = d + 1 - r;

    return array_row_col(r, c);
}
