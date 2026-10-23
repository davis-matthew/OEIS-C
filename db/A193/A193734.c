/*
 * A193734  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(2x+1)^n and q(n,x)=(x+2)^n.
 * https://oeis.org/A193734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangular array T(n,k) has T(n,k) = 4*T(n-1,k-1) + T(n-1,k)
 *            (k>=1) with T(0,0)=1, T(1,0)=1, T(1,1)=2. Sequence a(n) is the
 *            row‑major flattening of T(n,k)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 131, u64 n <= 457,
 *            u128 n <= 1325, big any n
 * Helpers:   ensure_rows_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_rows_capacity.h"

seq_t a(idx_t n)
{
    /* Static state: all rows that have been built */
    static seq_t **rows = NULL;     /* array of pointers to rows */
    static size_t rows_cap = 0;     /* current capacity of rows array */
    static size_t rows_count = 0;   /* number of rows currently stored */
    static idx_t  cumulative_terms = 0; /* how many terms have been stored */

    /* Build rows until we have at least n+1 terms */
    while (cumulative_terms <= n)
    {
        size_t r   = rows_count;        /* index of new row */
        size_t len = r + 1;             /* length of new row */
        seq_t *new_row = (seq_t *)malloc(len * sizeof(seq_t));

        if (r == 0) {
            /* Row 0 */
            new_row[0] = (seq_t)1;
        } else if (r == 1) {
            /* Row 1 (special base values) */
            new_row[0] = (seq_t)1;
            new_row[1] = (seq_t)2;
        } else {
            /* General recurrence */
            seq_t *prev = rows[r - 1];
            new_row[0] = prev[0];                 /* T(n,0) = T(n-1,0) */
            for (size_t k = 1; k <= r; ++k) {
                seq_t val = (seq_t)4 * prev[k - 1];
                if (k < len - 1)                  /* add T(n-1,k) if it exists */
                    val += prev[k];
                new_row[k] = val;
            }
        }

        /* Store the new row */
        ensure_rows_capacity(rows_count + 1, &rows, &rows_cap);
        rows[rows_count] = new_row;
        cumulative_terms += len;
        rows_count++;
    }

    /* Find the row containing term n */
    idx_t offset = 0;
    size_t row_idx = 0;
    while (offset + (row_idx + 1) <= n) {
        offset += row_idx + 1;
        row_idx++;
    }
    size_t col = (size_t)(n - offset);

    return rows[row_idx][col];
}
