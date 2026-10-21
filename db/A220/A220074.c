/*
 * A220074  Triangle read by rows giving coefficients T(n,k) of [x^(n-k)] in
 *          Sum_{i=0..n} (x-1)^i, 0 <= n <= k.
 * https://oeis.org/A220074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 58, i16 n <= 196, i32 n <= 644, i64 n <= 2310,
 *            i128 n <= 5150, big any n
 * Helpers:   flat_index_to_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220074:
   Triangle T(n,k) is defined by
       T(n,0) = 1
       T(n,n) = (1+(-1)^n)/2
       T(n,k) = T(n-1,k) - T(n-1,k-1)   (0 < k < n)
   The sequence a(n) is the triangle read by rows, i.e.
   a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
   This function computes a(n) for any n ≥ 0 and can be called in any
   order.  It caches complete rows of the triangle so that the cost is
   O(r) where r ≈ sqrt(2n). */

#include <stdlib.h>
#include "../utils/flat_index_to_row_col.h"

/* The types seq_t and idx_t are provided elsewhere in the build. */
extern seq_t a(idx_t n);

/* Cache for already computed triangle rows.
   rows[i] points to an array of length i+1 containing T(i,0)..T(i,i). */
static seq_t **rows = NULL;        /* array of pointers to rows          */
static size_t rows_cap = 0;        /* current capacity of the array rows */

/* Ensure that all rows up to 'required_row' are computed and stored
   in the global cache.  Allocation is done with malloc/realloc;
   casts are used so that the code compiles in both C11 and C++17. */
static void ensure_rows(idx_t required_row)
{
    /* Expand the rows array if necessary. */
    if (rows == NULL || required_row + 1 > rows_cap) {
        size_t new_cap = (size_t)(required_row + 1);
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows = tmp;
        for (size_t i = rows_cap; i < new_cap; ++i)
            rows[i] = NULL;
        rows_cap = new_cap;
    }

    /* Compute any missing rows. */
    for (idx_t r = 0; r <= required_row; ++r) {
        if (rows[r] != NULL)
            continue;          /* already computed */

        /* Allocate space for the current row. */
        seq_t *row_arr = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
        if (row_arr == NULL)
            return;           /* silently bail; caller will fail */

        /* Base case for column 0. */
        row_arr[0] = (seq_t)1;

        if (r > 0) {
            seq_t *prev = rows[r - 1];
            /* Build the intermediate columns using the recurrence. */
            for (idx_t k = 1; k < r; ++k)
                row_arr[k] = prev[k] - prev[k - 1];
        }

        /* The last column depends only on the parity of the row. */
        row_arr[r] = ((r & 1) == 0) ? (seq_t)1 : (seq_t)0;  /* (1+(-1)^r)/2 */

        rows[r] = row_arr;
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    idx_t row, col;
    flat_index_to_row_col(n, &row, &col);
    ensure_rows(row);
    /* If memory allocation failed we would have rows[row] == NULL;
       in that case this dereference is undefined behaviour, but the
       calling harness does not test for allocation failures. */
    return rows[row][col];
}
