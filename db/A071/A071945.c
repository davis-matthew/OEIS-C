/*
 * A071945  Triangle T(n,k) read by rows giving number of underdiagonal
 *          lattice paths from (0,0) to (n,k) using steps R=(1,0), V=(0,1) and
 *          D=(2,1).
 * https://oeis.org/A071945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 185, u64 n <= 558,
 *            u128 n <= 1949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula for the triangle:
       T(0,0) = 1
       T(n,k) = T(n,k-1) + T(n-1,k) + T(n-2,k-1)    for 0 ≤ k ≤ n
   a(n) is the element T(r,c) where r(r+1)/2 ≤ n < (r+1)(r+2)/2
   and c = n - r(r+1)/2.  The implementation memoises rows of
   the triangle up to the required index. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine row r and column c for the linear index n */
    idx_t r = 0;
    for (;; ++r) {
        idx_t tri = r * (r + 1) / 2;
        if (tri > n) break;
    }
    --r;
    idx_t tri_r = r * (r + 1) / 2;
    idx_t c = n - tri_r;

    /* Memoised rows of the triangle: static array of pointers to rows */
    static seq_t **rows = NULL;
    static idx_t rows_cnt = 0;

    /* Expand the table of row pointers as needed */
    if (rows_cnt <= r) {
        idx_t new_cnt = r + 1;
        seq_t **temp = (seq_t **)realloc(rows, new_cnt * sizeof(*rows));
        rows = temp;                   /* realloc may move the array */
        for (idx_t i = rows_cnt; i <= r; ++i) {
            /* allocate storage for row i (i+1 entries) */
            rows[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            /* compute the row via the recurrence */
            for (idx_t j = 0; j <= i; ++j) {
                seq_t val;
                if (j == 0) {
                    val = 1;                           /* T(i,0) = 1 */
                } else {
                    seq_t left = rows[i][j - 1];               /* T(i,j-1) */
                    seq_t up   = (j <= i - 1) ? rows[i - 1][j] : 0;  /* T(i-1,j) */
                    seq_t diag = (i >= 2 && j - 1 <= i - 2) ? rows[i - 2][j - 1] : 0; /* T(i-2,j-1) */
                    val = left + up + diag;
                }
                rows[i][j] = val;
            }
        }
        rows_cnt = new_cnt;
    }

    return rows[r][c];
}
