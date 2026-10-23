/*
 * A135494  Triangle read by rows: row n gives coefficients C(n,j) for a
 *          Sheffer sequence (binomial-type) with lowering operator (D-1)/2 +
 *          T{ (1/2) * exp[(D-1)/2] } where T(x) is Cayley's Tree function.
 * https://oeis.org/A135494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the row/col value c(r,j) of the triangle defined by *
 *            c(0,0) = 1 , c(0,k>0) = 0 * c(1,1) = 1 , l other c(1,k)=0 * for
 *            r >= 2: * c(r,j) = c(r-1,j-1) + j*c(r-1,j) - 2*(r-1)*c(r-2,j-1)
 *            * (terms with indices out of range are treated as 0) * a(n) is
 *            obtained by flattening the triangle row‑by‑r...
 * Range:     i8 n <= 33, i16 n <= 68, i32 n <= 157, i64 n <= 325,
 *            i128 n <= 325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but defensive */

    /* find row r: smallest r with r*(r+1)/2 >= n */
    uint64_t d = 8 * (uint64_t)n + 1;
    double sqrt_d = sqrt((double)d);
    idx_t r = (idx_t)((sqrt_d - 1.0) / 2.0);
    if ((uint64_t)r * (r + 1) / 2 < n) r++;

    /* column j (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t j = n - prev_tri;

    /* Special small cases */
    if (r == 1) { /* row 1 has only c(1,1)=1 */
        return (idx_t)1;
    }

    /* allocate rows r-2 and r-1 */
    seq_t *row_prev2 = (seq_t*)malloc(sizeof(seq_t) * 1);
    row_prev2[0] = (seq_t)1; /* row 0 */
    seq_t *row_prev1 = (seq_t*)malloc(sizeof(seq_t) * 2);
    row_prev1[0] = 0;
    row_prev1[1] = 1;        /* row 1 */

    seq_t *row_cur = NULL;

    for (idx_t cur = 2; cur <= r; ++cur) {
        size_t len = cur + 1;
        row_cur = (seq_t*)malloc(len * sizeof(seq_t));
        row_cur[0] = 0;

        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = row_prev1[k - 1];
            seq_t term2 = 0;
            seq_t term3 = 0;

            if (k < cur) {                      /* c(r-1,k) exists */
                term2 = (seq_t)k * row_prev1[k];
            }
            if (k - 1 <= cur - 2) {              /* c(r-2,k-1) exists */
                term3 = 2 * (seq_t)(cur - 1) * row_prev2[k - 1];
            }
            row_cur[k] = term1 + term2 - term3;
        }
        free(row_prev2);
        row_prev2 = row_prev1;
        row_prev1 = row_cur;
    }

    seq_t result = row_prev1[j];
    free(row_prev2);
    free(row_prev1);
    return result;
}
