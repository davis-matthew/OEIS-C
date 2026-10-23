/*
 * A140068  Triangle read by rows, n-th row = (n-1)-th power of the matrix X *
 *          [1,0,0,0,...] where X = an infinite lower triangular matrix with
 *          [1,2,1,2,1,2,...] in the main diagonal and [1,1,1,...] in the
 *          subdiagonal.
 * https://oeis.org/A140068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the rows of A140068:
   t[1][1] = 1;
   t[k][j] = (j%2?1:2) * t[k-1][j] + t[k-1][j-1]   (j>=1, t[*][0] = 0).
   The sequence a(n) is the triangle read by rows.  For a given linear
   index n (n ≥ 1) find the row r with T(r)=r(r+1)/2 ≥ n and the
   column c = n − T(r‑1).  Then a(n) = t[r][c]. */
#include <stdlib.h>   /* malloc, realloc */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find the row r such that T(r) = r(r+1)/2 ≥ n. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Column within that row. */
    idx_t c = n - ((r - 1) * r) / 2;

    /* Static cache of rows. */
    static seq_t **rows = NULL;
    static idx_t rows_len = 0;
    static idx_t rows_cap = 0;

    if (rows_len < r) {
        /* Grow the pointer array if necessary. */
        if (rows_cap < r + 1) {
            idx_t new_cap = rows_cap ? rows_cap * 2 : 4;
            if (new_cap < r + 1)
                new_cap = r + 1;
            rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            rows_cap = new_cap;
        }

        /* Compute and store new rows from rows_len+1 up to r. */
        for (idx_t k = rows_len + 1; k <= r; ++k) {
            seq_t *row = (seq_t*)malloc((k + 1) * sizeof(seq_t));
            row[0] = 0;   /* unused slot */

            if (k == 1) {
                row[1] = 1;
            } else {
                seq_t *prev = rows[k - 1];
                for (idx_t j = 1; j <= k; ++j) {
                    seq_t d = (j & 1) ? 1 : 2;          /* diagonal 1,2,1,2,... */
                    seq_t a_prev_j  = (j <= k - 1) ? prev[j] : 0;
                    seq_t a_prev_j1 = (j > 1) ? prev[j - 1] : 0;
                    row[j] = d * a_prev_j + a_prev_j1;
                }
            }
            rows[k] = row;
        }
        rows_len = r;
    }

    return rows[r][c];
}
