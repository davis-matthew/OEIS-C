/*
 * A144400  Triangle read by rows: row n (n > 0) gives the coefficients of x^k
 *          (0 <= k <= n - 1) in the expansion of Sum_{j=0..n}
 *          A000931(j+4)*binomial(n, j)*x^(j - 1)*(1 - x)^(n - j).
 * https://oeis.org/A144400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   Triangle read by rows: row n (n > 0) gives coefficients of x^k
 *            (0 ≤ k ≤ n−1) in the expansion ∑_{j=0..n}
 *            A000931(j+4)*binomial(n,j)*x^(j−1)*(1−x)^(n−j). The sequence
 *            a(n) is obtained by reading the triangle row‑wise starting with
 *            a(1)=1
 * Range:     i8 n <= 32, i16 n <= 84, i32 n <= 291, i64 n <= 1062,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_ROW 5000   /* enough for n up to about 1.25e7 */

static seq_t *tri[MAX_ROW + 1] = {0};
static idx_t  max_computed = 0;

/* multiply array a[0..m-1] by a polynomial coeff[0..deg]
   result is stored in res[0..res_len-1] (res_len ≥ m+deg) */
static void mult_poly(const seq_t *a, idx_t m,
                      const int *coeff, idx_t deg,
                      seq_t *res, idx_t res_len)
{
    /* res is assumed already zeroed for indices 0..res_len-1 */
    for (idx_t i = 0; i < m; ++i) {
        seq_t ai = a[i];
        for (idx_t k = 0; k <= deg; ++k) {
            int c = coeff[k];
            if (!c) continue;
            idx_t idx = i + k;
            if (idx >= res_len) continue;
            seq_t prod = (seq_t)c * ai;
            res[idx] += prod;
        }
    }
}

/* build rows up to 'row' (inclusive) using the recurrence derived from the
   generating function.  Base rows 1,2,3 are constructed directly. */
static void build_rows(idx_t row)
{
    if (row <= max_computed) return;

    for (idx_t n = max_computed + 1; n <= row; ++n) {
        idx_t len = n;                /* row n has n coefficients */
        tri[n] = (seq_t *)malloc(len * sizeof(seq_t));
        for (idx_t i = 0; i < len; ++i) tri[n][i] = 0;

        if (n == 1) {                 /* row 1 */
            tri[1][0] = 1;
        } else if (n == 2) {          /* row 2 derived from the recurrence */
            static const int coeff1[2] = {3, -3};   /* 3 - 3x */
            seq_t tmp[2] = {0, 0};
            mult_poly(tri[1], 1, coeff1, 1, tmp, 2);
            tri[2][0] = tmp[0] - 1;          /* add -1 to coefficient of x^0 */
            tri[2][1] = tmp[1] + 2;          /* add +2 to coefficient of x^1 */
        } else if (n == 3) {          /* row 3, explicit values */
            tri[3][0] = 3;
            tri[3][1] = -3;
            tri[3][2] = 1;
        } else {                      /* recurrence for n ≥ 4 */
            static const int coeff1[2] = {3, -3};        /* 3 - 3x */
            static const int coeff2[3] = {-3, 6, -2};    /* -3 + 6x - 2x^2 */
            static const int coeff3[4] = {1, -3, 2, 1};   /* 1 - 3x + 2x^2 + x^3 */

            seq_t tmp1[MAX_ROW] = {0};
            seq_t tmp2[MAX_ROW] = {0};
            seq_t tmp3[MAX_ROW] = {0};

            mult_poly(tri[n-1], n-1, coeff1, 1, tmp1, len);
            mult_poly(tri[n-2], n-2, coeff2, 2, tmp2, len);
            mult_poly(tri[n-3], n-3, coeff3, 3, tmp3, len);

            for (idx_t i = 0; i < len; ++i)
                tri[n][i] = tmp1[i] + tmp2[i] + tmp3[i];
        }
    }
    max_computed = row;
}

/* row r contains r coefficients.  Find smallest r such that r(r+1)/2 ≥ n,
   then column k = n - r(r-1)/2 - 1 (0‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n==0, but guard */

    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    idx_t prev_terms = r * (r - 1) / 2;
    idx_t k = n - prev_terms - 1;   /* 0‑based column */

    build_rows(r);                   /* ensure required row(s) built */
    return tri[r][k];
}
