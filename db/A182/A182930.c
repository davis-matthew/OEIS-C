/*
 * A182930  Triangle read by rows: Number of set partitions of {1,2,..,n} such
 *          that |k| is a block and no block |m| with m < k exists, (1 <= n, 1
 *          <= k <= n).
 * https://oeis.org/A182930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 136, u64 n <= 351,
 *            u128 n <= 946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for the triangle T(n,k):
        T(1,1) = 1
        T(n,n) = T(n-1,1) - T(n-1,n-1)   (n>1)
        T(n,k) = T(n,k+1) + T(n-1,k)      (1 ≤ k < n)
    The sequence a(m) is the triangle read by rows:
        a(1)   = T(1,1)
        a(2)   = T(2,1), a(3) = T(2,2)
        a(4)   = T(3,1), a(5) = T(3,2), a(6) = T(3,3), …
    This function returns a(n) = T(r,c) where r and c are found from
    the flattened index n (offset 1).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Map the linear index n to coordinates (row r, column c) */
    idx_t tot = 0;
    idx_t r = 0;
    while (tot < n) {
        r++;
        tot += r;
    }
    idx_t prev_tot = tot - r;
    idx_t c = n - prev_tot;   /* 1 ≤ c ≤ r */

    /* Static storage for dynamic programming of the triangle */
    static seq_t **tri = NULL;
    static idx_t max_rows = 0;

    /* Grow the table if needed */
    if (r > max_rows) {
        idx_t new_rows = r;
        idx_t *new_tri = (idx_t *)realloc(tri, (new_rows + 1) * sizeof(seq_t *));
        tri = (seq_t **)new_tri;
        for (idx_t i = max_rows + 1; i <= new_rows; i++) {
            tri[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            /* 1‑based indexing: tri[i][0] unused */
        }

        /* Compute rows that were not yet computed */
        for (idx_t m = max_rows + 1; m <= new_rows; m++) {
            if (m == 1) {
                tri[1][1] = (seq_t)1;
            } else {
                /* T(m,m) */
                tri[m][m] = tri[m - 1][1] - tri[m - 1][m - 1];
                /* Remaining columns, from right to left */
                for (idx_t k = m - 1; k >= 1; k--) {
                    tri[m][k] = tri[m][k + 1] + tri[m - 1][k];
                }
            }
        }
        max_rows = new_rows;
    }

    return tri[r][c];
}
