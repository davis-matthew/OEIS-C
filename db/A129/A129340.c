/*
 * A129340  Triangular array read by rows: for n, k >= 1, a(n+1, 1) = 2*a(n,
 *          n); a(n+1, k+1) = a(n, k)+a(n+1, k).
 * https://oeis.org/A129340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129340: Triangular array read by rows
   Recurrence (OEIS definition):
     T(1,1)      = 1
     T(n+1,1)    = 2 * T(n,n)
     T(n+1,k+1)  = T(n,k) + T(n+1,k)   (k ≥ 1)
   The sequence a(n) is the n-th element read row‑by‑row:
     a(n) = T(r,k),  where r = smallest integer with r(r+1)/2 ≥ n
     and k = n - r(r-1)/2.  */

#include <stddef.h>
#include <stdlib.h>

static inline idx_t triangular_idx(idx_t r)
{
    /* r*(r+1)/2 using idx_t to avoid overflow for r up to ~10^9. */
    return r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    /* static state: array of pointers to each row, one row per index r (1‑based). */
    static seq_t **rows = NULL;    /* rows[i] points to row (i+1) of length (i+1) */
    static size_t rows_cap = 0;    /* current capacity of rows array */
    static size_t rows_len = 0;    /* number of rows currently computed */

    /* Grow rows to contain enough rows to reach index n. */
    while (triangular_idx((idx_t)rows_len) < n)
    {
        size_t new_len = rows_len + 1;         /* length of the new row (r = new_len) */
        seq_t *row = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!row) { abort(); }                /* Out of memory */

        if (rows_len == 0) {
            /* First row: T(1,1) = 1 */
            row[0] = 1;
        } else {
            /* Compute first element: 2 * last element of previous row */
            row[0] = 2 * rows[rows_len - 1][rows_len - 1];
            /* Compute remaining elements */
            for (size_t i = 1; i < new_len; ++i) {
                row[i] = rows[rows_len - 1][i - 1] + row[i - 1];
            }
        }

        /* Ensure capacity for the rows array */
        if (rows_len >= rows_cap) {
            size_t new_cap = rows_cap == 0 ? 4 : rows_cap << 1;
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) { abort(); }
            rows = tmp;
            rows_cap = new_cap;
        }

        rows[rows_len] = row;
        rows_len = new_len;
    }

    /* Now rows_len rows are available, and triangular(rows_len) ≥ n. */
    idx_t r = rows_len;                           /* 1‑based row index                 */
    idx_t offset = triangular_idx(r - 1);         /* elements before this row          */
    idx_t k = n - offset;                         /* 1‑based position within the row   */

    return rows[r - 1][k - 1];
}
