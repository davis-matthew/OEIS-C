/*
 * A053122  Triangle of coefficients of Chebyshev's S(n,x-2) = U(n,x/2-1)
 *          polynomials (exponents of x in increasing order).
 * https://oeis.org/A053122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 94, i32 n <= 309, i64 n <= 1193,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle a(n,m):
 * a(n,m) = -2*a(n-1,m) + a(n-1,m-1) - a(n-2,m)
 * with a(0,0)=1, a(1,0)=-2, a(1,1)=1.
 * The function a(n) returns the flattening of this triangle
 * (row-major order, n = 0,1,2,3,…). */

#include <stddef.h>
#include <stdlib.h>

/* Allocate a new array of seq_t of given length.
 * The callers are responsible for freeing the returned pointer. */
static seq_t* seq_alloc_new_array(size_t len)
{
#ifndef __cplusplus
    /* If seq_t is a plain arithmetic type, malloc/calloc works. */
    return (seq_t*)calloc(len, sizeof(seq_t));
#else
    /* In C++ builds seq_t might be a big‑int class; use new. */
    return new seq_t[len]();
#endif
}

/* Cached rows of the triangle. */
static seq_t **rows = NULL;          /* array of pointers to each row */
static size_t rows_cap = 0;          /* capacity of the pointer array   */
static size_t rows_len = 0;          /* number of rows actually created */

/* Ensure that rows[0..r_needed] exist. The rows are built incrementally
 * starting from rows[0] and rows[1]. */
static void ensure_rows(size_t r_needed)
{
    /* Initial allocation of row pointers. */
    if (rows_cap == 0) {
        rows_cap = 2;
        rows = (seq_t**)calloc(rows_cap, sizeof(seq_t*));
    }
    /* Grow the pointer array if necessary. */
    if (rows_cap <= r_needed) {
        size_t new_cap = rows_cap * 2;
        if (new_cap <= r_needed) new_cap = r_needed + 1;
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows_cap = new_cap;
    }

    /* Create row 0 if not yet present. */
    if (rows_len == 0) {
        rows[0] = seq_alloc_new_array(1);
        rows[0][0] = (seq_t)1;
        rows_len = 1;
    }
    /* Create row 1 if not yet present. */
    if (rows_len == 1 && r_needed >= 1) {
        rows[1] = seq_alloc_new_array(2);
        rows[1][0] = (seq_t)-2;
        rows[1][1] = (seq_t)1;
        rows_len = 2;
    }

    /* Build further rows up to r_needed. */
    while (rows_len <= r_needed) {
        size_t r = rows_len;      /* zero‑based row index */
        size_t len = r + 1;
        seq_t *row = seq_alloc_new_array(len);

        /* Compute each entry of the new row using the recurrence. */
        for (size_t m = 0; m < len; ++m) {
            seq_t val;
            if (m == 0) {
                /* a(r,0) = -2*a(r-1,0) - a(r-2,0) */
                val = - ( (seq_t)2 * rows[r-1][0] ) - rows[r-2][0];
            } else if (m == r) {
                /* a(r,r) = a(r-1,r-1) */
                val = rows[r-1][m-1];
            } else {
                /* a(r,m) = -2*a(r-1,m) + a(r-1,m-1) - a(r-2,m) */
                seq_t t1 = rows[r-1][m];                 /* a(r-1,m)  */
                seq_t t2 = rows[r-1][m-1];               /* a(r-1,m-1)* */
                seq_t t3 = (m <= r-2) ? rows[r-2][m] : (seq_t)0; /* a(r-2,m) or 0 */
                val = t2 - t3 - ( (seq_t)2 * t1 );
            }
            row[m] = val;
        }

        rows[rows_len] = row;
        ++rows_len;
    }
}

/* Convert a linear index n to (row r, column c) in the triangle. */
static void index_to_rc(idx_t n, idx_t *r_out, idx_t *c_out)
{
    /* Solve r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t s = 8ULL * n + 1;                      /* fits in 64‑bit */
    idx_t sq = 0;
    /* integer square root of s */
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t t = r | ((idx_t)1 << i);
        if ((__int128)t * t <= s) r = t;
    }
    sq = r;
    *r_out = (sq - 1) >> 1;
    idx_t off = (idx_t)(((__int128)(*r_out) * (*r_out + 1)) >> 1); /* r*(r+1)/2 */
    *c_out = n - off;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rc(n, &r, &c);
    ensure_rows((size_t)r);
    return rows[r][c];
}
