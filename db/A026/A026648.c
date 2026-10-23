/*
 * A026648  Triangular array T read by rows: T(n,0)=T(n,n)=1 for n >= 0; for n
 *          >= 2 and 1<=k<=n-1, T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k) if n is
 *          even and k is odd, else T(n,k)=t(n-1,k-1)+T(n-1,k).
 * https://oeis.org/A026648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element (1‑based) of the triangular array * T
 *            defined by T(0,0)=1, T(n,0)=T(n,n)=1 for n>0, * for n>=2,
 *            1<=k<=n-1: * if (n even and k odd)
 *            T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k) * else
 *            T(n,k)=T(n-1,k-1)+T(n-1,k). * The sequence a(n) reads T row by
 *            row starting with T(0,0). This * i...
 * Range:     u8 n <= 59, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoised storage of the rows of T: rows[r] points to an array of
 * (r+1) seq_t values.  rows_len is the number of rows currently
 * stored.  rows_cap is the allocated capacity of the rows array. */
static seq_t **rows      = NULL;
static size_t rows_len   = 0;
static size_t rows_cap   = 0;

/* Ensure that rows[0 .. r] are computed (and thus available).  The
 * function enlarges the rows array and the row buffers as needed. */
static void ensure_rows(idx_t r)
{
    /* Grow the array that stores the row pointers. */
    while (rows_len <= (size_t)r) {
        if (rows_len >= rows_cap) {
            size_t new_cap = rows_cap ? (rows_cap * 2) : 4;
            if (rows_len + 1 > new_cap) new_cap = rows_len + 1;
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            if (!tmp) abort();          /* fatal allocation error */
            rows      = tmp;
            rows_cap  = new_cap;
        }

        /* Compute the next row. */
        size_t rr = rows_len;
        seq_t *new_row = (seq_t*)malloc((rr + 1) * sizeof(seq_t));
        if (!new_row) abort();

        if (rr == 0) {
            new_row[0] = 1;
        } else if (rr == 1) {
            new_row[0] = 1;
            new_row[1] = 1;
        } else {
            new_row[0] = 1;
            new_row[rr] = 1;
            for (size_t k = 1; k < rr; ++k) {
                if ((rr % 2 == 0) && (k % 2 == 1)) {
                    /* T(rr,k)=T(rr-1,k-1)+T(rr-2,k-1)+T(rr-1,k) */
                    new_row[k] = rows[rr-1][k-1] + rows[rr-2][k-1] + rows[rr-1][k];
                } else {
                    /* T(rr,k)=T(rr-1,k-1)+T(rr-1,k) */
                    new_row[k] = rows[rr-1][k-1] + rows[rr-1][k];
                }
            }
        }

        rows[rows_len] = new_row;
        ++rows_len;
    }
}

/* Return the nth term of A026648 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for the offset */

    /* Find the row index r such that the row contains the nth term.
     * Row r has length r+1 and the total number of terms up to row r
     * is (r+1)(r+2)/2.  We find the smallest r with that count >= n. */
    size_t r = 0;
    while (((seq_t)(r + 1) * (r + 2)) / 2 < n) {
        ++r;
    }

    /* Compute the first index of this row (1‑based). */
    idx_t start = (idx_t)r * (r + 1) / 2 + 1;
    /* Index within the row (0‑based). */
    idx_t col   = n - start;

    /* Ensure rows up to r are available. */
    ensure_rows((idx_t)r);

    return rows[r][(size_t)col];
}
