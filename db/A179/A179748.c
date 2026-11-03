/*
 * A179748  Triangle T(n,k) read by rows. T(n,1)=1, k > 1: T(n,k) =
 *          Sum_{i=1..k-1} T(n-i,k-1).
 * https://oeis.org/A179748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,1)=1; for k>1: T(n,k)=∑_{i=1}^{k-1} T(n-i,k-1)   */
/* Sequence a(n) is the flattened triangle T(n,1),T(n,2),… by rows.   */
/* Offset 1 – a(1) is the first term.                                 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return the linear index in the flat 'triangle' array for entry (row,col).
   Both row and col are 1‑based. */
static inline size_t idx_table(idx_t row, idx_t col) {
    /* (row‑1)*row/2 + (col‑1) */
    return (size_t)((row - 1ULL) * (row) / 2 + (col - 1ULL));
}

/* Flat storage of all triangle values. */
static seq_t *triangle = NULL;
/* Largest row that has been generated. */
static idx_t max_rows = 0;
/* Allocated number of seq_t elements in 'triangle'. */
static size_t capacity = 0;

/* Ensure the triangle array can hold at least 'rows' rows. */
static void ensure_rows(idx_t rows) {
    if (rows <= max_rows)
        return;

    /* New required capacity. */
    size_t new_cap = (size_t)rows * (rows + 1) / 2;
    size_t old_cap = capacity;

    if (new_cap > capacity) {
        seq_t *tmp = mem_resize(seq_t, triangle, new_cap);
        triangle = tmp;
        capacity = new_cap;
    }

    /* Initialize newly allocated elements to zero. */
    for (size_t i = old_cap; i < new_cap; ++i)
        triangle[i] = 0;

    /* Compute triangle entries for the new rows. */
    for (idx_t r = max_rows + 1; r <= rows; ++r) {
        /* k = 1 */
        triangle[idx_table(r, 1)] = 1;

        /* k ≥ 2 */
        for (idx_t k = 2; k <= r; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i < k; ++i) {
                /* Only use valid indices: n-i ≥ k-1. */
                if (r - i >= k - 1) {
                    sum += triangle[idx_table(r - i, k - 1)];
                }
            }
            triangle[idx_table(r, k)] = sum;
        }
    }

    max_rows = rows;
}

/* Return the n‑th term of sequence A179748.  */
seq_t a(idx_t n) {
    /* Find the row that contains the n‑th term. */
    idx_t row = 1;
    while ((row * (row + 1) / 2) < n)
        ++row;

    /* Ensure all rows up to 'row' are computed. */
    ensure_rows(row);

    /* Column within the row. */
    idx_t prev_rows = (row - 1) * row / 2;  /* number of terms before this row */
    idx_t col = n - prev_rows;              /* 1‑based column */

    return triangle[idx_table(row, col)];
}
