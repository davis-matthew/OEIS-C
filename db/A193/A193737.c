/*
 * A193737  Mirror of the triangle A193736.
 * https://oeis.org/A193737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the greatest integer with r(r+1)/2 ≤ n,
 *            k = n – r(r+1)/2. The triangle T is defined by T(0,0) = 1,
 *            T(1,0) = T(1,1) = 1, T(2,0) = 1, T(n,k) = T(n-1,k) + T(n-1,k-1)
 *            + T(n-2,k) for n ≥ 3 and 0 ≤ k ≤ n
 * Range:     u8 n <= 46, u16 n <= 124, u32 n <= 414, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   row_col_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "../utils/row_col_of.h"

/* Thread‑local triangle storage */
static seq_t **triangle = NULL;       /* array of pointers to rows          */
static size_t row_ptr_cap = 0;        /* capacity of the row pointer array  */
static idx_t  rows_allocated = 0;     /* number of rows actually filled     */

/* Ensure that rows 0..needed exist in the triangle */
static void ensure_triangle(idx_t needed)
{
    if (needed < rows_allocated)
        return;

    /* Grow the row pointer array if necessary */
    if (needed >= row_ptr_cap) {
        size_t new_cap = (row_ptr_cap > 0) ? row_ptr_cap : 1;
        while (new_cap <= (size_t)needed)
            new_cap <<= 1;
        seq_t **tmp = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
        assert(tmp != NULL);
        triangle = tmp;
        row_ptr_cap = new_cap;
    }

    /* Compute missing rows, starting after the last allocated one */
    for (idx_t n = rows_allocated; n <= needed; ++n) {
        size_t len = (size_t)(n + 1);
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        assert(row != NULL);
        triangle[n] = row;

        if (n == 0) {
            row[0] = (seq_t)1;
        } else if (n == 1) {
            row[0] = (seq_t)1;
            row[1] = (seq_t)1;
        } else if (n == 2) {
            row[0] = (seq_t)1;
            row[1] = (seq_t)2;
            row[2] = (seq_t)1;
        } else {
            for (idx_t k = 0; k <= n; ++k) {
                seq_t v = (seq_t)0;
                if (k <= n-1) v += triangle[n-1][k];
                if (k > 0)    v += triangle[n-1][k-1];
                if (k <= n-2) v += triangle[n-2][k];
                row[k] = v;
            }
        }
    }

    rows_allocated = needed + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_of(n, &r, &k);
    ensure_triangle(r);
    return triangle[r][k];
}
