/*
 * A096589  Symmetric square array T(n,k)=T(k,n), read by antidiagonals, such
 *          that T(n,k) equals the dot product of the k-th antidiagonal with
 *          the initial terms of the (n-k)-th row when n>=k, with T(n,0)=1.
 * https://oeis.org/A096589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_row, alloc_rows, free_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096589 – symmetric square array
 *
 * a(n) = T(m-k,k)   where m*(m+1)/2 <= n < (m+1)*(m+2)/2
 *
 * T(n,0) = 1
 * T(n,k) = sum_{j=0..k} T(k-j,j) * T(n-k,j)   (for n >= k)
 * T(n,k) = T(k,n)                              (for n <  k)
 *
 * T is symmetric and read by antidiagonals; the first element of each
 * antidiagonal is T(m,0)=1, the last element is T(0,m)=1.  The above
 * recurrence is used to build all entries row‑by‑row.
 *
 * The implementation keeps a lazy triangular table of T.  The table
 * is grown only to the size required by the current call, and the
 * values are computed directly from previously computed rows.
 */

#include <stdlib.h>
#include "../utils/alloc_row.h"
#include "../utils/alloc_rows.h"
#include "../utils/free_rows.h"

/* Global storage for the triangular table of T.
 * Trows[r] points to an array of length r+1 that holds T(r,0), … ,T(r,r).   */
static seq_t **Trows = NULL;          /* array of row pointers             */
static idx_t  Trows_size = 0;         /* number of rows allocated (size)   */

/* Forward declaration: grow the table up to row 'nmax' (inclusive). */
static void ensure_rows(idx_t nmax);

/* Direct, non‑recursive accessor for a previously computed T(i,j).  It
 * is safe to call only for indices that are already present in the
 * table; the caller must guarantee that. */
static inline seq_t getT_local(idx_t i, idx_t j)
{
    /* Handle the trivial column j==0. */
    if (j == 0) return (seq_t)1;

    /* Reflect across the diagonal: the table is symmetric. */
    if (i == 0 && j > 0)        /* T(0,j) = T(j,0) = 1 */
        return (seq_t)1;

    if (i < j) {
        idx_t t = i;
        i = j;
        j = t;
    }

    /* Now i >= j >= 1  and the required row exists. */
    return Trows[i][j];
}

/* Grow the triangular table up to row nmax (inclusive). */
static void ensure_rows(idx_t nmax)
{
    if (nmax < Trows_size)
        return;               /* already large enough */

    size_t new_size = (size_t)nmax + 1;
    seq_t **new_rows = alloc_rows(new_size);
    if (!new_rows) exit(1);    /* allocation failed */

    /* Copy existing row pointers. */
    for (idx_t r = 0; r < Trows_size; ++r)
        new_rows[r] = Trows[r];

    /* Build new rows one by one.  All needed dependencies are in
     * rows that are already computed. */
    for (idx_t r = Trows_size; r <= nmax; ++r) {
        seq_t *row = alloc_row(r + 1);
        if (!row) exit(1);

        row[0] = (seq_t)1;       /* base case T(r,0)=1 */

        for (idx_t k = 1; k <= r; ++k) {        /* compute T(r,k) */
            seq_t sum = (seq_t)0;
            for (idx_t t = 0; t <= k; ++t) {
                seq_t a = getT_local(k - t, t);      /* T(k-t, t) */
                seq_t b = getT_local(r - k, t);      /* T(r-k, t) */
                sum += a * b;
            }
            row[k] = sum;
        }
        new_rows[r] = row;
    }

    /* Replace the old pointer array.  The old array itself is freed;
     * the row data remains valid because it is now referenced through
     * new_rows. */
    if (Trows)
        free_rows(Trows);
    Trows = new_rows;
    Trows_size = new_size;
}

/* Public entry point – compute the n‑th term of A096589. */
seq_t a(idx_t n)
{
    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2.  Using a
     * simple binary search is fast enough for the required range. */
    idx_t low = 0, high = n, m = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = (mid * (mid + 1ULL)) / 2ULL;
        if (tri <= n) {
            m = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Ensure all rows up to 'm' have been computed. */
    ensure_rows(m);

    idx_t tri_m = (m * (m + 1ULL)) / 2ULL;
    idx_t k     = n - tri_m;          /* position inside the antidiagonal */
    idx_t i     = m - k;              /* T(i,k) = T(m-k,k) */

    return getT_local(i, k);
}
