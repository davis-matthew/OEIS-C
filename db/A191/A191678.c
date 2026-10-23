/*
 * A191678  Number of lattice paths from (0,0) to (n,n) using steps (1,0),
 *          (1,1), (0,2), (2,2).
 * https://oeis.org/A191678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191678: number of lattice paths from (0,0) to (n,n) using steps
   (1,0), (1,1), (0,2), (2,2).
   Recurrence:
       f[i][0] = 1  (for all i ≥ 0)
       f[0][j] = 1  if j even, else 0
       for i ≥ 1, j ≥ 1:
           f[i][j] = f[i-1][j]           // (1,0)
                   + f[i-1][j-1]         // (1,1)
                   + (j≥2 ? f[i][j-2] : 0)   // (0,2)
                   + (i≥2 && j≥2 ? f[i-2][j-2] : 0) // (2,2)
   a(n) = f[n][n].
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate three rows of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *rows[3];
    rows[0] = (seq_t *)malloc(sz * sizeof(seq_t));
    rows[1] = (seq_t *)malloc(sz * sizeof(seq_t));
    rows[2] = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!rows[0] || !rows[1] || !rows[2]) {
        /* Allocation failed – return 0 as a failure indicator. */
        return (seq_t)0;
    }

    /* Row i = 0 */
    for (size_t j = 0; j < sz; ++j) {
        rows[0][j] = (j == 0) ? (seq_t)1 : (((j & 1u) == 0) ? (seq_t)1 : (seq_t)0);
    }

    if (n == 0) {
        seq_t res = rows[0][0];
        free(rows[0]); free(rows[1]); free(rows[2]);
        return res;
    }

    /* Row i = 1 */
    for (size_t j = 0; j < sz; ++j) {
        if (j == 0) {
            rows[1][j] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            sum += rows[0][j];          /* (1,0) */
            sum += rows[0][j - 1];      /* (1,1) */
            if (j >= 2) sum += rows[1][j - 2]; /* (0,2) */
            rows[1][j] = sum;
        }
    }

    if (n == 1) {
        seq_t res = rows[1][1];
        free(rows[0]); free(rows[1]); free(rows[2]);
        return res;
    }

    /* Rows for i ≥ 2, cycling through the three buffers */
    for (idx_t i = 2; i <= n; ++i) {
        int cur   = (int)(i % 3);
        int prev  = (int)((i - 1) % 3);
        int prev2 = (int)((i - 2) % 3);

        for (size_t j = 0; j < sz; ++j) {
            if (j == 0) {
                rows[cur][j] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            sum += rows[prev][j];        /* (1,0) */
            sum += rows[prev][j - 1];    /* (1,1) */
            if (j >= 2) sum += rows[cur][j - 2]; /* (0,2) */
            if (i >= 2 && j >= 2) sum += rows[prev2][j - 2]; /* (2,2) */
            rows[cur][j] = sum;
        }
    }

    seq_t res = rows[n % 3][n];
    free(rows[0]); free(rows[1]); free(rows[2]);
    return res;
}
