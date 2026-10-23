/*
 * A129065  Coefficients of the v=1 member of a family of certain orthogonal
 *          polynomials.
 * https://oeis.org/A129065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the coefficient T(r,m) of the orthogonal polynomial
 *            p(r,1,x). The row index r is the unique integer with r(r+1)/2 ≤
 *            n < (r+1)(r+2)/2, and the column index m is m = n - r(r+1)/2.
 *            T(r,m) is computed from the recurrence T(0,0)=1 T(-1,·)=0
 *            T(n,m)=T(n-1,m-1)+2*(n-1)²*T(n-1,m)-(n-1)²*(n-2)²*T(n...
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 45, u64 n <= 105,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the row r that contains index n */
    idx_t r = 0;
    while ((seq_t)(r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* Compute m = n - row_offset */
    seq_t offset = (seq_t)r * (r + 1) / 2;
    idx_t m = (idx_t)(n - offset);

    /* Special case: row 0 */
    if (r == 0) {
        return (seq_t)1;      /* T(0,0) = 1 */
    }

    /* Allocate three rows of maximum length r+1 */
    idx_t maxlen = r + 1;
    seq_t *row_prev2 = (seq_t *)malloc(sizeof(seq_t) * maxlen); /* row n-2 */
    seq_t *row_prev1 = (seq_t *)malloc(sizeof(seq_t) * maxlen); /* row n-1 */
    seq_t *row_cur   = (seq_t *)malloc(sizeof(seq_t) * maxlen); /* current row */

    /* Initialize all to zero */
    for (idx_t i = 0; i < maxlen; i++) {
        row_prev2[i] = 0;
        row_prev1[i] = 0;
        row_cur[i]   = 0;
    }

    /* Row 0 */
    row_prev1[0] = 1;   /* T(0,0) */

    /* Iterate up to row r */
    for (idx_t cur = 1; cur <= r; cur++) {
        seq_t nn = (seq_t)(cur - 1);
        seq_t coeff1 = 2 * nn * nn;                             /* 2*(n-1)^2   */
        seq_t coeff2 = nn * nn * (seq_t)(cur - 2) * (seq_t)(cur - 2); /* (n-1)^2*(n-2)^2 */

        for (idx_t j = 0; j <= cur; j++) {
            seq_t val = 0;

            if (j > 0) {
                val += row_prev1[j - 1];
            }
            if (j <= cur - 1) {
                val += coeff1 * row_prev1[j];
            }
            if (cur >= 2 && j <= cur - 2) {
                val -= coeff2 * row_prev2[j];
            }
            row_cur[j] = val;
        }

        /* Rotate rows: prev2 <- prev1, prev1 <- cur, cur <- prev2 */
        seq_t *tmp = row_prev2;
        row_prev2  = row_prev1;
        row_prev1  = row_cur;
        row_cur    = tmp;
    }

    /* row_prev1 now holds row r */
    seq_t result = row_prev1[m];

    /* Free allocated memory */
    free(row_prev2);
    free(row_prev1);
    free(row_cur);

    return result;
}
