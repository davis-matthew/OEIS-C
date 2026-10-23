/*
 * A172303  Table T(n,k) with coefficients [x^k] of the polynomials p(x,n) =
 *          x^(n-1)*p(x,n-1) + x^(n-2)*p(x,n-2), recurrence starting p(x,0)=0,
 *          p(x,1)=1.
 * https://oeis.org/A172303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=0, a(1)=1; a(n) is the nth entry of the flattened table *
 *            of coefficients T(n,k) of the polynomials * p(x,0)=0, p(x,1)=1,
 *            * p(x,n)=x^(n‑1)*p(x,n‑1)+x^(n‑2)*p(x,n‑2), n≥2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of rows: rows[i] holds the coefficients of p(x,i+1)    */
    static idx_t      max_row   = (idx_t)-1;           /* last computed row index  */
    static seq_t **   rows      = NULL;                /* array of pointers to rows   */
    static size_t *   row_len   = NULL;                /* number of coefficients per row   */
    static idx_t *    cum_len   = NULL;                /* cumulative elements up to each row */

    /* initialise cache for the first call */
    if (max_row == (idx_t)-1) {
        max_row = 0;
        rows = (seq_t **)malloc(sizeof(seq_t *) * 1);
        row_len = (size_t *)malloc(sizeof(size_t) * 1);
        cum_len = (idx_t *)malloc(sizeof(idx_t) * 1);

        rows[0] = (seq_t *)malloc(sizeof(seq_t) * 1);   /* p(x,0) = 0 */
        rows[0][0] = 0;
        row_len[0] = 1;
        cum_len[0] = 1;
    }

    /* grow cache until the cumulative length covers n */
    while (cum_len[max_row] <= n) {
        idx_t r = max_row + 1;
        size_t L = (size_t)(r * (r - 1ULL) / 2ULL + 1);   /* length of this row   */
        seq_t *row = (seq_t *)malloc(L * sizeof(seq_t));

        if (r == 1) {                /* p(x,1) = 1 */
            row[0] = 1;
        } else {
            /* build row r using the recurrence */
            for (size_t k = 0; k < L; ++k) {
                seq_t val = 0;
                /* term from p(x,r-1) shifted by (r-1) */
                if (k >= (size_t)(r - 1) && (k - (r - 1)) < row_len[r - 1])
                    val += rows[r - 1][k - (r - 1)];
                /* term from p(x,r-2) shifted by (r-2) */
                if (k >= (size_t)(r - 2) && (k - (r - 2)) < row_len[r - 2])
                    val += rows[r - 2][k - (r - 2)];
                row[k] = val;
            }
        }

        /* append new row */
        rows = (seq_t **)realloc(rows, sizeof(seq_t *) * (r + 1));
        row_len = (size_t *)realloc(row_len, sizeof(size_t) * (r + 1));
        cum_len = (idx_t *)realloc(cum_len, sizeof(idx_t) * (r + 1));

        rows[r] = row;
        row_len[r] = L;
        cum_len[r] = cum_len[max_row] + (idx_t)L;

        max_row = r;
    }

    /* locate row and position inside that row */
    idx_t r = max_row;
    idx_t prev_cum = (r == 0) ? 0 : cum_len[r - 1];
    size_t k = (size_t)(n - prev_cum);

    return rows[r][k];
}
