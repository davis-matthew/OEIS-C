/*
 * A157012  Riordan's general Eulerian recursion: T(n,k) = (k+2)*T(n-1, k) +
 *          (n-k) * T(n-1, k-1), with T(n,0) = 1, T(n,n) = 0.
 * https://oeis.org/A157012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading Riordan's triangle row‑wise. The
 *            triangle T(n,k) is defined by T(n,0) = 1, T(n,n) = 0, T(n,k) =
 *            (k+2)*T(n-1,k) + (n-k)*T(n-1,k-1) (0 < k < n)
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 95, u64 n <= 238,
 *            u128 n <= 609, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* static storage for the rows of the triangle */
static seq_t **rows = NULL;
static size_t  rows_cap = 0;     /* capacity of the rows array          */
static idx_t   rows_count = 0;   /* number of rows already computed      */

/* Ensure that the rows array can hold indices up to 'needed'. */
static void ensure_rows_capacity(idx_t needed)
{
    if ((size_t)needed < rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap << 1 : 8;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    rows = tmp;
    rows_cap = new_cap;
}

/* Compute rows from rows_count up to and including 'r'. */
static void compute_rows_up_to(idx_t r)
{
    for (idx_t m = rows_count; m <= r; ++m) {
        ensure_rows_capacity(m);
        seq_t *cur = (seq_t *)malloc((size_t)(m + 1) * sizeof(seq_t));
        rows[m] = cur;
        if (m == 0) {
            cur[0] = 1;
            continue;
        }
        const seq_t *prev = rows[m - 1];
        cur[0] = 1;    /* T(m,0)  = 1   */
        cur[m] = 0;    /* T(m,m)  = 0   */
        for (idx_t k = 1; k < m; ++k) {
            /* Use the generic arithmetic of seq_t; for builtin types
               this may overflow for very large values, but the
               sequence is verified only up to the point where the result
               fits the type. */
            seq_t term1 = prev[k] * (seq_t)(k + 2);
            seq_t term2 = prev[k - 1] * (seq_t)(m - k);
            cur[k] = term1 + term2;
        }
    }
    rows_count = r + 1;
}

/* Find row 'r' that contains the n-th term and the cumulative
   number of terms before that row.  */
static void locate_row(idx_t n, idx_t *row, idx_t *cum_before)
{
    idx_t r = 0;
    idx_t cum = 0;
    /* cum holds number of elements in rows 0 .. r-1 */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    *row = r;
    *cum_before = cum;
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    idx_t r, cum;
    locate_row(n, &r, &cum);
    if (r >= rows_count)
        compute_rows_up_to(r);
    idx_t col = n - cum;
    return rows[r][col];
}
