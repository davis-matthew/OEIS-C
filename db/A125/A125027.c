/*
 * A125027  Binomial transform of the "1,2,3,..." triangle.
 * https://oeis.org/A125027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   * a(n) = Σ_{k=j}^{i} binom(i-1, k-1) * ( (k-1)*k/2 + j ), *
 *            where i and j are determined by the position of n in the
 *            triangle: * cumulative(i-1) = (i-1)*i/2 (number of terms before
 *            row i), * column j = n - cumulative(i-1) (1‑based column in row
 *            i). * * The function computes this directly using th...
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but safe to return 0 */
        return 0;

    /* Find the row i such that the nth term lies in it. */
    idx_t row = 1;
    idx_t cumulative = 1;            /* number of terms up to current row */
    while (cumulative < n) {
        ++row;
        cumulative += row;
    }

    /* Compute the column index j (1‑based) within that row. */
    idx_t prev = (row - 1) * row / 2;   /* terms before this row */
    idx_t col = n - prev;               /* position inside the row */

    /* Sum over k = col .. row of binom(i-1, k-1) * ((k-1)*k/2 + col) */
    seq_t result = 0;
    for (idx_t k = col; k <= row; ++k) {
        seq_t bin = binom(row - 1, k - 1);
        seq_t mk  = (((seq_t)(k - 1) * k) / 2) + col;
        result += bin * mk;
    }

    return result;
}
