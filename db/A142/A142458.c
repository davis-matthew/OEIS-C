/*
 * A142458  Triangle T(n,k) read by rows: T(n,k) = 1 if k=1 or k=n, otherwise
 *          T(n,k) = (3*n-3*k+1)*T(n-1,k-1) + (3*k-2)*T(n-1,k).
 * https://oeis.org/A142458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = 1 if k=1 or k=n * T(n,k) = (3*n-3*k+1)*T(n-1,k-1) +
 *            (3*k-2)*T(n-1,k) * a(n) is the flattened triangle: a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(2,2), … * The nth term equals T(r,k) where r
 *            is the smallest integer with * r*(r+1)/2 ≥ n (row count), and k
 *            = n - r*(r-1)/2 (column index, 1‑based)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 59, u64 n <= 142,
 *            u128 n <= 388, big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"   /* provides seq_alloc_array() */

/* static cache for the triangle rows */
static seq_t **rows      = NULL;   /* rows[1] … rows[max_row] */
static size_t  rows_cap  = 0;      /* allocated length of rows[]   */
static idx_t   max_row   = 0;      /* highest row computed */

/* ensure rows array can hold up to `need` rows (rows[0] unused) */
static void ensure_rows_capacity(idx_t need)
{
    if (need < rows_cap) return;
    size_t new_cap = rows_cap > 0 ? rows_cap : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    rows_cap = new_cap;
}

/* precompute all rows up to and including `r` */
static void ensure_rows(idx_t r)
{
    if (r <= max_row) return;

    ensure_rows_capacity(r);

    for (idx_t i = max_row + 1; i <= r; ++i)
    {
        /* allocate row i: indices 1 … i, index 0 unused */
        rows[i] = seq_alloc_array((size_t)(i + 1));

        /* first and last elements are 1 */
        rows[i][1] = (seq_t)1;
        if (i > 1) rows[i][i] = (seq_t)1;

        /* interior elements */
        if (i > 2)
        {
            for (idx_t k = 2; k <= i - 1; ++k)
            {
                seq_t c1 = (seq_t)(3 * i - 3 * k + 1) * rows[i - 1][k - 1];
                seq_t c2 = (seq_t)(3 * k - 2) * rows[i - 1][k];
                rows[i][k] = c1 + c2;
            }
        }
    }
    max_row = r;
}

/* compute the nth term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    /* find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* compute all rows up to r */
    ensure_rows(r);

    /* compute column index k within the row (1‑based) */
    idx_t start = ((r - 1) * r) / 2 + 1;        /* first index of row r */
    idx_t k     = n - start + 1;                /* column within row */

    return rows[r][k];
}
