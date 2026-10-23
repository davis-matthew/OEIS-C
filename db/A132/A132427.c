/*
 * A132427  Triangle, read by rows of 2n+1 terms, where T(n,k) = T(n,k-1) +
 *          T(n-1,k-2) for n>0, 1<k<=2n, with T(n,1)=T(n,0)=T(n-1,2n-2) for
 *          n>0 and T(0,0)=1.
 * https://oeis.org/A132427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0,0)=1;
   for n>0, T(n,0)=T(n,1)=T(n-1,2n-2);
   for n>0, 1<k<=2n, T(n,k)=T(n,k-1)+T(n-1,k-2).
   a(n) is the element T(r,k) with r=floor(sqrt(n)) and k=n-r*r. */

#include <stdlib.h>

/* Static cache of computed rows. */
static seq_t **rows = NULL;       /* array of pointers to each row */
static size_t  rows_count = 0;    /* number of rows currently stored */
static size_t  rows_cap   = 0;    /* current capacity of the rows array */

/* Grow the rows array and compute any missing rows up to r. */
static void ensure_rows_to(idx_t r)
{
    /* Grow the rows array if necessary */
    if (rows_cap <= (size_t)r) {
        size_t new_cap = rows_cap == 0 ? 4 : rows_cap * 2;
        while (new_cap <= (size_t)r) new_cap *= 2;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) return;               /* allocation failure – caller will handle zero */
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute missing rows up to r */
    while (rows_count <= (size_t)r) {
        size_t n = rows_count;          /* current row number */
        size_t len = 2 * n + 1;         /* number of terms in row n */

        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) return;               /* allocation failure */

        if (n == 0) {
            row[0] = 1;                  /* base case */
        } else {
            seq_t prev_last = rows[n-1][2 * (n-1)]; /* T(n-1,2n-2) */
            row[0]   = prev_last;
            row[1]   = prev_last;
            for (size_t k = 2; k < len; ++k) {
                row[k] = row[k-1] + rows[n-1][k-2];
            }
        }

        rows[n] = row;
        rows_count++;
    }
}

seq_t a(idx_t n)
{
    /* Find row r: r^2 <= n < (r+1)^2  (i.e. r = floor(sqrt(n))) */
    idx_t r = 0;
    while (r + 1 <= n / (r + 1)) ++r;

    /* Ensure rows up to r are computed */
    ensure_rows_to(r);
    if (rows_count <= (size_t)r || !rows[r]) return 0; /* safety */

    /* Column index within row r */
    idx_t k = n - r * r;

    return rows[r][(size_t)k];
}
