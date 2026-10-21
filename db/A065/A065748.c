/*
 * A065748  Triangle of Gandhi polynomial coefficients.
 * https://oeis.org/A065748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the (i,j)-th entry of the Gandhi polynomial triangle
 *            where i and j satisfy n = i*(3*i-1)/2 + j + 1 (i.e. the
 *            cumulative count of rows.) The recursive definition of the
 *            coefficients is implemented using the recurrence: coeff[i][k] =
 *            Σ_{m=k}^{deg_prev} coeff[i-1][m] * binom(m, k) - (k≥4 ? c...
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

static seq_t **triangle = NULL;     /* triangle[i] holds row i (1‑indexed)   */
static idx_t *row_len   = NULL;     /* number of coefficients in each row   */
static idx_t rows_allocated = 0;    /* highest i that has been computed      */

/* compute and cache rows 1..up_to */
static void ensure_rows(idx_t up_to)
{
    if (up_to <= rows_allocated) return;

    triangle  = (seq_t **)realloc(triangle,
                                 (up_to + 1) * sizeof(seq_t *));
    row_len   = (idx_t *)realloc(row_len,
                                 (up_to + 1) * sizeof(idx_t));

    for (idx_t i = rows_allocated + 1; i <= up_to; ++i) {
        idx_t L = 3 * i - 2;                /* length of row i   */
        seq_t *row = (seq_t *)calloc(L, sizeof(seq_t));
        if (!row) return;                  /* allocation failure – abort */

        if (i == 1) {                      /* B(X,1)=X^4 => one coeff 1   */
            row[0] = 1;
        } else {
            idx_t prev = i - 1;
            seq_t *prev_row = triangle[prev];
            idx_t L_prev = row_len[prev];
            idx_t deg_prev = 4 + (L_prev - 1);  /* max exponent in prev */

            for (idx_t k = 0; k < L; ++k) {
                seq_t sum = 0;
                idx_t m_start = k;
                if (m_start < 4) m_start = 4;
                for (idx_t m = m_start; m <= deg_prev; ++m) {
                    seq_t coeff = prev_row[m - 4];
                    sum += coeff * binom(m, k);
                }
                if (k >= 4)      /* subtract coeff of X^k in previous row */
                    sum -= prev_row[k - 4];
                row[k] = sum;
            }
        }

        triangle[i]  = row;
        row_len[i]   = L;
    }
    rows_allocated = up_to;
}

/* cumulative number of terms up to and including row r */
static idx_t cumulative(idx_t r)
{
    return r * (3 * r - 1) / 2;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined offset */

    /* find row that contains term n */
    idx_t r = 1;
    while (cumulative(r) < n) ++r;

    idx_t prev_cum = cumulative(r - 1);
    idx_t j = n - prev_cum - 1;                /* 0‑based column index */

    ensure_rows(r);
    return triangle[r][j];
}
