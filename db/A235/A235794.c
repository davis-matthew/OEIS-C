/*
 * A235794  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k
 *          starts with k zeros and then lists the odd numbers interleaved
 *          with k zeros, and the first element of column k is in row
 *          k(k+1)/2.
 * https://oeis.org/A235794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the number at index n read row‑by‑row in the * triangle
 *            T(n,k) where column k starts with k zeros and then lists * odd
 *            numbers interleaved with k zeros, the first element of column *
 *            k appearing in row k(k+1)/2. The row length is * row_len(r) =
 *            max{k | k(k+1)/2 ≤ r} = ⌊(√(8r+1) – 1)/2⌋. T...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   row_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_len.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first element of row 1. */
    if (n == 0)    /* undefined in OEIS, return 0 for safety */
        return (seq_t)0;

    /* Find the row that contains the n‑th term. */
    idx_t row = 0, cum = 0;
    while (cum < n) {
        ++row;
        cum += row_len(row);
    }

    /* Number of terms before this row. */
    idx_t len_cur = row_len(row);
    idx_t before  = cum - len_cur;

    /* Column index (1‑based) within the current row. */
    idx_t col = n - before;          /* 1 ≤ col ≤ len_cur */
    idx_t k   = col;

    /* First row of column k. */
    idx_t start_row = (idx_t)(((__int128)k * (k + 1)) / 2);

    /* Position of the term in column k (1‑based). */
    idx_t p = row - start_row + 1;

    if (p <= k)          /* one of the initial k zeros */
        return (seq_t)0;

    /* Offset from the first odd number (0‑based). */
    idx_t offset  = p - k - 1;
    idx_t mod_sz  = k + 1;
    idx_t pos     = offset % mod_sz;   /* 0 → odd, otherwise zero */

    if (pos == 0) {
        seq_t odd = (seq_t)(2 * (seq_t)(offset / mod_sz) + 1);
        return odd;
    } else {
        return (seq_t)0;
    }
}
