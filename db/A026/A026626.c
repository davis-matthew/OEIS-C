/*
 * A026626  Triangular array T read by rows: T(n,0) = T(n,n) = 1 for n >= 0,
 *          T(n,1) = T(n,n-1) = floor(3*n/2) for n >= 1, T(n,k) = T(n-1,k-1) +
 *          T(n-1,k) for 2 <= k <= n-2, n >= 4.
 * https://oeis.org/A026626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read by rows from the triangle T defined by * T(0,0)=1;
 *            * T(n,0)=T(n,n)=1 for n>=0; * T(n,1)=T(n,n-1)=floor(3*n/2) for
 *            n>=1; * T(n,k)=T(n-1,k-1)+T(n-1,k) for 2<=k<=n-2, n>=4. * This
 *            triangle is stored row by row; a(n) = T(r,c) where r and c are *
 *            obtained by unfolding the index n into the...
 * Range:     u8 n <= 58, u16 n <= 179, u32 n <= 644, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed rows of the triangle */
    static seq_t **rows = NULL;
    static idx_t   rows_cnt = 0;   /* number of rows currently cached   */

    /* Find the row r and column c for the flat index n */
    idx_t r = 0;
    while (1) {
        /* (r+1)*(r+2)/2 uses 128‑bit to avoid overflow for large n */
        unsigned __int128 next = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (next > n) break;
        r++;
    }
    idx_t c = n - (unsigned __int128)r * (r + 1) / 2;

    /* Extend the cache up to row r if necessary */
    if (r >= rows_cnt) {
        idx_t new_cnt = r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cnt * sizeof(seq_t *));
        if (!tmp) return 0;   /* out of memory; safe fallback */
        rows = tmp;

        for (idx_t i = rows_cnt; i < new_cnt; ++i) {
            /* allocate storage for row i (i+1 elements) */
            rows[i] = (seq_t*)malloc((size_t)(i + 1) * sizeof(seq_t));
            if (!rows[i]) return 0;   /* out of memory; safe fallback */

            /* set the first and last element to 1 */
            rows[i][0] = (seq_t)1;
            rows[i][i] = (seq_t)1;

            if (i >= 1) {
                /* T(i,1) = floor(3*i/2) */
                seq_t val = (seq_t)3 * (seq_t)i / (seq_t)2;
                rows[i][1] = val;
                rows[i][i - 1] = val;   /* symmetry */
            }

            /* interior elements, only for i >= 4 */
            if (i >= 4) {
                for (idx_t k = 2; k <= i - 2; ++k) {
                    rows[i][k] = rows[i - 1][k - 1] + rows[i - 1][k];
                }
            }
        }
        rows_cnt = new_cnt;
    }

    return rows[r][c];
}
