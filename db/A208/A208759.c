/*
 * A208759  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208760; see the Formula section.
 * https://oeis.org/A208759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the flattened triangle T(n,k) defined by
 *            T(0,0) = 1; T(n,0) = 1; T(n,k) = T(n-1,k) + 2*T(n-1,k-1) +
 *            2*T(n-2,k-2) (n≥1, k≥1). The first few rows are 1; 1, 2; 1, 4,
 *            6; 1, 6, 16, 16; 1, 8, 30, 56, 44; etc. The sequence a(n) is the
 *            linearization of this triangle, starting with...
 * Range:     u8 n <= 25, u16 n <= 62, u32 n <= 204, u64 n <= 729,
 *            u128 n <= 2680, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"

/* static storage for already computed rows of the triangle */
static seq_t **rows = NULL;          /* array of pointers to rows      */
static size_t rows_cap = 0;          /* allocated capacity of rows     */
static idx_t rows_created = 0;       /* number of rows actually created (max index +1) */

/* ensure that rows[0..r] exist, creating missing rows sequentially */
static void ensure_rows(idx_t r)
{
    if (r < rows_created)
        return;                     /* already available */

    /* grow the rows array if needed */
    size_t new_cap = rows_cap ? rows_cap : 1;
    while (new_cap <= (size_t)r)
        new_cap <<= 1;
    seq_t **new_rows = (seq_t**) realloc(rows, new_cap * sizeof(seq_t*));
    if (!new_rows) abort();          /* allocation failure */
    for (size_t i = rows_cap; i < new_cap; ++i)
        new_rows[i] = NULL;
    rows = new_rows;
    rows_cap = new_cap;

    /* create missing rows sequentially */
    for (idx_t i = rows_created; i <= r; ++i) {
        size_t len = (size_t)(i + 1);            /* row i has i+1 coefficients */
        seq_t *arr = seq_alloc_new_array(len);   /* provided by utils */
        if (!arr) abort();

        /* first coefficient is always 1 */
        arr[0] = (seq_t)1;

        if (i >= 1) {
            for (idx_t k = 1; k <= i; ++k) {
                seq_t v1 = 0, v2 = 0, v3 = 0;

                /* T(i-1,k) */
                if (k <= (idx_t)(i - 1))
                    v1 = rows[i - 1][k];

                /* 2*T(i-1,k-1) */
                if (k >= 1 && (k - 1) <= (idx_t)(i - 1))
                    v2 = (seq_t)2 * rows[i - 1][k - 1];

                /* 2*T(i-2,k-2) */
                if (k >= 2 && (k - 2) <= (idx_t)(i - 2))
                    v3 = (seq_t)2 * rows[i - 2][k - 2];

                arr[k] = v1 + v2 + v3;
            }
        }

        rows[i] = arr;
        rows_created = i + 1;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)               /* out of range: defined only for n≥1 */
        return (seq_t)0;

    /* find the triangular row r such that (r+1)(r+2)/2 >= n */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 < n )
        ++r;

    /* number of terms before row r */
    idx_t terms_before = r * (r + 1) / 2;
    idx_t k = n - terms_before - 1;      /* 0‑based column within the row */

    ensure_rows(r);
    return rows[r][k];
}
