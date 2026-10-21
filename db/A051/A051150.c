/*
 * A051150  Triangle read by rows: T(n, m) = S1(n, m)*5^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the triangle read by rows of T(n,m) = T(n-1,m-1) –
 *            5·(n-1)·T(n-1,m), with T(1,1) = 1; a(n) is the n‑th term of this
 *            triangle
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Accept only positive indices; undefined behaviour for n==0, but return 0. */
    if (n == 0)
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cumulative = 1;          /* number of terms up to current row */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column index within row r (1‑based). */
    idx_t col = n - (r - 1) * r / 2;

    /* Allocate two rows of length r+1 (we ignore index 0). */
    seq_t *prev = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!prev || !cur) {
        /* Memory allocation failed; return 0 (should not happen in tests). */
        free(prev);
        free(cur);
        return 0;
    }

    /* Row 1: only one element. */
    prev[1] = 1;

    /* Build rows 2 … r using the recurrence. */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t factor = (seq_t)(5 * (i - 1));   /* 5·(i‑1) fits in seq_t for required ranges */

        /* First element of row i */
        cur[1] = -(factor * prev[1]);

        /* Middle elements */
        for (idx_t j = 2; j < i; ++j) {
            cur[j] = prev[j - 1] - factor * prev[j];
        }

        /* Last element of row i */
        cur[i] = 1;

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Retrieve the desired term from the computed row. */
    seq_t result = prev[col];

    /* Free allocated memory. */
    free(prev);
    free(cur);
    return result;
}
