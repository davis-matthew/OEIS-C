/*
 * A239098  Triangle read by rows: T(0,0)=1; T(m,0)=0; otherwise T(m,n) =
 *          (m-1)*T(m-1,n)+(m-1+n)*T(m-1,n-1).
 * https://oeis.org/A239098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,0)=1; T(m,0)=0 for m>0;
 *            T(m,n)=(m-1)*T(m-1,n)+(m-1+n)*T(m-1,n-1)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the triangle read by rows (triangular indexing). */

#include <stdlib.h>

/* cache of computed rows; rows[m][n] = T(m,n) for 0 ≤ n ≤ m */
static seq_t **rows = NULL;          /* pointer array of rows */
static size_t num_rows = 0;          /* number of rows currently stored (max index + 1) */
static size_t rows_cap = 0;          /* capacity of the rows pointer array */

/* allocate or grow the rows cache up to at least mmax */
static void ensure_rows(size_t mmax)
{
    if (num_rows > mmax)
        return;

    /* Grow the rows pointer array if necessary */
    if (rows_cap < mmax + 1) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 4;
        while (new_cap < mmax + 1)
            new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            exit(1);   /* out of memory */
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute any missing rows */
    for (size_t m = num_rows; m <= mmax; ++m) {
        size_t len = m + 1;
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row)
            exit(1);   /* out of memory */

        if (m == 0) {
            row[0] = (seq_t)1;
        } else {
            row[0] = (seq_t)0;   /* T(m,0)=0 for m>0 */
            for (idx_t n = 1; n <= (idx_t)m; ++n) {
                seq_t term1 = 0;
                if ((idx_t)n <= (idx_t)(m - 1)) {
                    term1 = (seq_t)(m - 1) * rows[m - 1][(size_t)n];
                }
                seq_t term2 = (seq_t)(m - 1 + n) * rows[m - 1][(size_t)(n - 1)];
                row[(size_t)n] = term1 + term2;
            }
        }
        rows[(size_t)m] = row;
    }
    num_rows = mmax + 1;
}

/* a(n) returns the sequence value at index n (zero‑based). */
seq_t a(idx_t n)
{
    /* Find the row m such that start <= n < next_start, where
       start = m*(m+1)/2 and next_start = (m+1)*(m+2)/2. */
    idx_t m = 0;
    while (( (unsigned __int128)(m + 1) * (m + 2) ) / 2 <= (unsigned __int128)n)
        ++m;

    idx_t start = (m * (m + 1)) / 2;
    idx_t col   = n - start;

    ensure_rows((size_t)m);
    return rows[(size_t)m][(size_t)col];
}
