/*
 * A053207  Rows of triangle formed using Pascal's rule except begin n-th row
 *          with n and end it with n+1.
 * https://oeis.org/A053207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(0)=0.
   For each natural r>=1 build a row of length r+1 where
     - the first element is 2r‑1,
     - the last element is 2r,
     - interior elements are the sum of the two adjacent elements
       from the previous row.
   The sequence is the row elements read row by row starting from row 0. */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = (idx_t)((sqrt(8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* Adjust for rounding errors */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    idx_t start = r * (r + 1) / 2;      /* index of the first element of this row */
    idx_t col = n - start;             /* column within the row (0‑based) */

    /* Build rows up to r using two temporary buffers */
    seq_t *prev = NULL;
    seq_t *curr = NULL;
    size_t prev_len = 0, curr_len = 0;

    /* Row 0 */
    prev_len = 1;
    prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
    if (!prev) return (seq_t)0;  /* allocation failed, return 0 */
    prev[0] = 0;                 /* only element of row 0 */

    if (r == 0) {
        seq_t res = prev[col];
        free(prev);
        return res;
    }

    /* Build successive rows */
    for (idx_t row = 1; row <= r; ++row) {
        curr_len = row + 1;
        curr = (seq_t *)malloc(curr_len * sizeof(seq_t));
        if (!curr) {
            free(prev);
            return (seq_t)0;
        }

        /* Edges */
        curr[0] = (seq_t)(2 * row - 1);
        curr[row] = (seq_t)(2 * row);

        /* Interior elements by Pascal's rule */
        for (idx_t k = 1; k < row; ++k) {
            curr[k] = prev[k - 1] + prev[k];
        }

        /* Prepare for next iteration */
        free(prev);
        prev = curr;
        prev_len = curr_len;
        curr = NULL;
    }

    seq_t result = prev[col];
    free(prev);
    return result;
}
