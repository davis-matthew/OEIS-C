/*
 * A187888  Riordan matrix (1/sqrt(1-4*x),x/(1-x)).
 * https://oeis.org/A187888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = a(row, col) of the Riordan triangle with
 *            a(row,0)=C(2*row,row) and a(row,k)=a(row-1,k)+a(row-1,k-1). row
 *            = floor((sqrt(8*n+1)-1)/2), col = n - row*(row+1)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"      /* binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Determine the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n) ++r;
    --r;                                   /* now r is the correct row */

    /* Column within the row */
    idx_t j = n - r * (r + 1) / 2;

    /* Static storage for the triangle */
    static seq_t **triangle = NULL;        /* triangle[i] points to row i array */
    static idx_t rows_allocated = 0;       /* number of rows currently allocated */

    /* Allocate missing rows up to r */
    if (r >= rows_allocated) {
        idx_t new_rows = r + 1;
        triangle = (seq_t **)realloc(triangle, new_rows * sizeof(seq_t *));
        if (!triangle) return 0;          /* allocation failure */

        /* Build new rows */
        for (idx_t i = rows_allocated; i <= r; ++i) {
            triangle[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            if (!triangle[i]) return 0;

            /* Column 0: central binomial coefficient */
            triangle[i][0] = binom(2 * i, i);

            /* Remaining columns by recurrence a(i,k)=a(i-1,k)+a(i-1,k-1) */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t left  = (k <= i - 1)   ? triangle[i-1][k]   : 0;
                seq_t right = (k-1 <= i - 1) ? triangle[i-1][k-1] : 0;
                triangle[i][k] = left + right;
            }
        }
        rows_allocated = new_rows;
    }

    return triangle[r][j];
}
