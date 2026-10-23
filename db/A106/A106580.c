/*
 * A106580  Triangle T(n, k) = T(n, k-1) + Sum_{i >= 1} T(n-2i, k-i) with
 *          T(n,0)=1, read by rows.
 * https://oeis.org/A106580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = T(n,k-1) + Σ_{i≥1} T(n-2i,k-i), T(n,0)=1. a(n) is the
 *            n-th element of the triangle read by rows
 * Range:     u8 n <= 61, u16 n <= 150, u32 n <= 491, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/row_of_index.h"

/* Static storage for computed rows.  rows[i] points to an array of length i+1. */
static seq_t **rows = NULL;      /* pointer array */
static idx_t  rows_count = 0;    /* number of rows stored */
static idx_t  rows_cap   = 0;    /* capacity of the pointer array */

/* Ensure rows has room for indices up to max_index (inclusive). */
static void ensure_rows_cap(idx_t max_index)
{
    if (max_index < rows_cap)
        return;
    /* Grow capacity: double until sufficient. */
    idx_t new_cap = rows_cap ? rows_cap * 2 : 16;
    while (new_cap <= max_index)
        new_cap *= 2;
    seq_t **new_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    if (!new_rows) abort();          /* out of memory */
    rows = new_rows;
    rows_cap = new_cap;
}

/* Compute row n (0‑based) and store it in rows[n].  Assumes all rows < n are ready. */
static void compute_row(idx_t n)
{
    /* Allocate storage for the new row.  */
    seq_t *row = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!row) abort();              /* out of memory */

    /* Base case: first entry is always 1.  */
    row[0] = (seq_t)1;

    if (n > 0) {
        /* Temporary array collecting the Σ_{i≥1} T(n-2i,k-i) contributions. */
        seq_t *S = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!S) abort();

        /* For all i such that n-2i ≥ 0, accumulate contributions from row n-2i. */
        for (idx_t i = 1; i <= n / 2; ++i) {
            idx_t m = n - 2 * i;               /* lower row index */
            seq_t *row_m = rows[m];            /* m < n, already computed */
            for (idx_t k = 0; k <= m; ++k) {
                S[k + i] += row_m[k];
            }
        }

        /* Fill the current row using the recurrence. */
        for (idx_t k = 1; k <= n; ++k) {
            row[k] = row[k - 1] + S[k];
        }

        free(S);
    }

    /* Store the computed row. */
    rows[n] = row;
    rows_count = n + 1;
}

/* Public function to return a(n).  */
seq_t a(idx_t n)
{
    /* Determine the row containing index n. */
    idx_t r = row_of_index(n);

    /* Compute all rows up to r. */
    if (rows_count <= r) {
        ensure_rows_cap(r);
        for (idx_t i = rows_count; i <= r; ++i)
            compute_row(i);
    }

    /* Compute column offset. */
    idx_t start = r * (r + 1) / 2;
    idx_t col   = n - start;

    return rows[r][col];
}
