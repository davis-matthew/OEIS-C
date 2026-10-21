/*
 * A222405  Triangle read by rows: left and right edges are A002061
 *          (1,3,7,13,21,...), interior entries are filled in using the Pascal
 *          triangle rule.
 * https://oeis.org/A222405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from a triangular array T. T[0][0] = 1. For r>0,
 *            T[r][0] = T[r][r] = r^2 + r + 1. For 0<j<r, T[r][j] =
 *            T[r-1][j-1] + T[r-1][j]. a(n) = T[row][col] where row satisfies
 *            row*(row+1)/2 ≤ n < (row+1)*(row+2)/2 and col = n -
 *            row*(row+1)/2
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row that contains index n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* Cache of already computed rows. */
    static seq_t **triangle = NULL;
    static idx_t rows_alloc = 0;

    /* Allocate more rows if needed. */
    if (rows_alloc <= r) {
        idx_t old_rows = rows_alloc;
        rows_alloc = r + 1;
        triangle = (seq_t **)realloc(triangle,
                                     (size_t)rows_alloc * sizeof(seq_t *));

        for (idx_t i = old_rows; i < rows_alloc; ++i) {
            triangle[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));

            /* Edge value for this row. */
            seq_t edge = (seq_t)i * (seq_t)i + (seq_t)i + 1;

            for (idx_t j = 0; j <= i; ++j) {
                if (j == 0 || j == i) {
                    triangle[i][j] = edge;
                } else {
                    triangle[i][j] =
                        triangle[i - 1][j - 1] + triangle[i - 1][j];
                }
            }
        }
    }

    /* Position within the row. */
    idx_t start = r * (r + 1) / 2;
    idx_t j = n - start;
    return triangle[r][j];
}
