/*
 * A100326  Triangle, read by rows, where row n equals the inverse binomial of
 *          column n of square array A100324, which lists the
 *          self-convolutions of SHIFT(A003169).
 * https://oeis.org/A100326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 91, u64 n <= 299,
 *            u128 n <= 1034, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th element of the triangle T read by rows.
 * T is defined by
 *   T(0,0) = 1
 *   T(n,0) = Σ_{k=0}^{n-1} (k+1)·T(n-1,k)          (n>0)
 *   T(n,k) = Σ_{i=0}^{n-k} T(i+1,0)·T(n-i-1,k-1)   (k>0)
 *
 * a(n) = T(r,c) where r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2
 * and c = n – r(r+1)/2.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for rows of the triangle. */
    static seq_t **rows   = NULL;   /* array of pointers to row arrays   */
    static size_t rows_cap = 0;     /* allocated capacity for 'rows'    */
    static size_t rows_used = 0;    /* number of rows actually computed  */

    /* Find the row 'r' that contains index n. */
    idx_t r = 0;
    /* Use unsigned __int128 to avoid overflow while searching. */
    unsigned __int128 t;
    while ((t = (unsigned __int128)(r + 1) * (r + 2) / 2) <= n) {
        ++r;
    }

    /* Grow the array of row pointers if necessary. */
    if (rows_used <= r) {
        if (rows_cap <= (size_t)r) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 1;
            while (new_cap <= (size_t)r) new_cap *= 2;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            rows = tmp;
            rows_cap = new_cap;
        }
        /* Compute all missing rows up to 'r'. */
        for (idx_t cur = (idx_t)rows_used; cur <= r; ++cur) {
            size_t len = (size_t)cur + 1;
            seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));

            /* T(cur,0) */
            if (cur == 0) {
                row[0] = (seq_t)1;
            } else {
                seq_t sum0 = (seq_t)0;
                seq_t *prev = rows[cur - 1];
                for (idx_t k = 0; k < cur; ++k) {
                    sum0 += ((seq_t)(k + 1)) * prev[k];
                }
                row[0] = sum0;
            }

            /* Keep the first element in a local variable to avoid
             * accessing rows[cur][0] before it is stored in the
             * ‘rows’ array. */
            seq_t cur_first = row[0];

            /* Remaining columns. */
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t sumk = (seq_t)0;
                for (idx_t i = 0; i <= cur - k; ++i) {
                    seq_t a_val = (i + 1 == cur) ? cur_first : rows[i + 1][0];
                    sumk += a_val * rows[cur - i - 1][k - 1];
                }
                row[k] = sumk;
            }

            rows[cur] = row;
        }
        rows_used = r + 1;
    }

    /* Locate the element within the found row. */
    unsigned __int128 start_of_row = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)start_of_row;
    return rows[r][c];
}
