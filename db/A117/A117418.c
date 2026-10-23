/*
 * A117418  Triangle T, read by rows, such that column 2k+1 of T equals column
 *          k of T^2 and column 2k of T equals column k of T*R: [T^2](n+k,k) =
 *          T(n+2k+1,2k+1) and [T*R](n+k,k) = T(n+2k,2k) for n>=0, k>=0, where
 *          R = SHIFT_RIGHT(T).
 * https://oeis.org/A117418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from triangle T whose entries T(n,k) are defined by
 *            * T(n,0) = T(n,n) = 1 * For odd columns: T(n,2k+1) =
 *            Σ_{j=0}^{n-2k-1} T(n-k-1,k+j) * T(k+j,k) * For even columns:
 *            T(n,2k) = Σ_{j=0}^{n-2k} T(n-k,k+j) * T(k-1+j,k-1) * The
 *            sequence is obtained by reading the triangle row‑by‑row
 * Range:     u8 n <= 36, u16 n <= 68, u32 n <= 171, u64 n <= 435,
 *            u128 n <= 1178, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
seq_t a(idx_t n)
{
    static seq_t **triangle = NULL;  /* triangular array: triangle[row][col] */
    static size_t rows = 0;          /* number of computed rows */
 
    /* Find the row r that contains index n (0‑based) and the column offset. */
    idx_t r = 0;
    idx_t offset = n;
    while (1) {
        idx_t len = r + 1;                 /* length of current row */
        if (offset < len) break;
        offset -= len;
        r++;
    }
 
    /* Expand the triangle until we have row r. */
    if ((idx_t)rows <= r) {
        for (idx_t m = rows; m <= r; ++m) {
            /* Allocate space for row m */
            triangle = (seq_t **)realloc(triangle, (size_t)(m + 1) * sizeof(seq_t *));
            triangle[m] = (seq_t *)malloc((size_t)(m + 1) * sizeof(seq_t));
 
            for (idx_t k = 0; k <= m; ++k) {
                /* Base cases */
                if (k == 0 || k == m) {
                    triangle[m][k] = (seq_t)1;
                    continue;
                }
 
                if (k & 1) { /* odd column: 2c+1 */
                    idx_t c = k / 2;
                    seq_t sum = 0;
                    for (idx_t j = 0; j <= m - 2 * c - 1; ++j) {
                        seq_t prod = triangle[m - c - 1][c + j] *
                                      triangle[c + j][c];
                        sum += prod;
                    }
                    triangle[m][k] = sum;
                } else { /* even column: 2c */
                    idx_t c = k / 2;
                    seq_t sum = 0;
                    for (idx_t j = 0; j <= m - 2 * c; ++j) {
                        seq_t prod = triangle[m - c][c + j] *
                                      triangle[c - 1 + j][c - 1];
                        sum += prod;
                    }
                    triangle[m][k] = sum;
                }
            }
        }
        rows = r + 1;
    }
 
    return triangle[r][offset];
}
