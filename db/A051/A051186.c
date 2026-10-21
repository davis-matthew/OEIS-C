/*
 * A051186  Triangle read by rows: T(n, m) = S1(n, m)*7^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the (n‑th) element of the triangle T(r,c) defined by
 *            T(r,c) = S1(r,c) * 7^(r–c), where S1 are the signed Stirling
 *            numbers of the first kind. For a flat index n, let r be the
 *            smallest integer with r(r+1)/2 ≥ n and c = n – r(r–1)/2. Then
 *            a(n) = T(r,c)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 28, i64 n <= 91, i128 n <= 253,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and column c corresponding to the n‑th flattened index. */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n)
        r++;
    idx_t c = n - (r - 1) * r / 2;   /* 1 <= c <= r */

    /* Base case: the first row is just [1]. */
    if (r == 1)
        return (seq_t)1;

    /* Allocate two rows of length r + 1 (index 0 unused). */
    size_t len = (size_t)(r + 1);
    seq_t *row_prev = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *row_cur  = (seq_t *)malloc(len * sizeof(seq_t));
    if (!row_prev || !row_cur) {
        free(row_prev);
        free(row_cur);
        return (seq_t)0;   /* allocation failure sentinel */
    }

    /* Initialise the first row. */
    for (size_t i = 0; i <= r; ++i)
        row_prev[i] = 0;
    row_prev[1] = (seq_t)1;   /* T(1,1) = 1 */

    /* Compute rows 2 .. r using the recurrence
       T(i,m) = T(i‑1,m‑1) – 7·(i‑1)·T(i‑1,m). */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t factor = (seq_t)7 * (seq_t)(i - 1);
        for (idx_t m = 1; m <= i; ++m) {
            seq_t prev_mminus1 = (m > 1) ? row_prev[m - 1] : 0;
            seq_t prev_m      = (m <= i - 1) ? row_prev[m] : 0;
            row_cur[m] = prev_mminus1 - factor * prev_m;
        }
        /* Swap rows for the next iteration. */
        seq_t *tmp = row_prev;
        row_prev = row_cur;
        row_cur  = tmp;
    }

    /* The requested entry is now in row_prev[c]. */
    seq_t result = row_prev[c];

    free(row_prev);
    free(row_cur);
    return result;
}
