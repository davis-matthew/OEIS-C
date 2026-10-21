/*
 * A225200  Triangle (read by rows) of coefficients of the polynomials (in
 *          ascending order) of the denominators of the generalized sequence
 *          of fractions f(n) defined recursively by f(1) = m/1; f(n+1) is
 *          chosen so that the sum and the product of the first n terms of the
 *          sequence are equal.
 * https://oeis.org/A225200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the flattening of the triangle of coefficients of the
 *            polynomials p_1(x) = 1 p_{n+1}(x) = x^{2^{n-1}} –
 *            p_1(x)p_2(x)…p_n(x) (n≥1) The triangle is read by rows, each row
 *            containing the coefficients of p_n(x) in ascending order of
 *            degree
 * Range:     i8 n <= 24, i16 n <= 44, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Temporary storage for the polynomials of each row.
   index 0 is unused; rows are numbered starting at 1. */
#define MAX_ROWS 128
static seq_t *row_poly[MAX_ROWS];
static idx_t row_len[MAX_ROWS];
static idx_t cumulative[MAX_ROWS];   /* cumulative number of terms up to row r */
static idx_t rows_done = 0;          /* number of rows already generated */

/* Current product P(x) = p_1(x) p_2(x) … p_{rows_done}(x) */
static seq_t *P = NULL;
static idx_t P_deg = 0;

/* Build rows up to containing at least the term with index n. */
static void build_up_to(idx_t n)
{
    /* Initialisation of the first row. */
    if (rows_done == 0) {
        cumulative[0] = 0;
        /* row 1: coefficients {1} */
        row_poly[1] = (seq_t *)malloc(sizeof(seq_t));
        row_poly[1][0] = (seq_t)1;
        row_len[1] = 1;
        cumulative[1] = 1;
        rows_done = 1;

        /* copy of the current product P(x) = p_1(x) */
        P = (seq_t *)malloc(sizeof(seq_t));
        P[0] = (seq_t)1;
        P_deg = 0;
    }

    /* Generate rows until we reach the required index n. */
    while (cumulative[rows_done] < n) {
        /* Current row index is rows_done, next row will be rows_done+1 */
        idx_t cur = rows_done;          /* corresponds to p_cur(x) */
        idx_t next = cur + 1;

        /* exponent 2^{cur-1} */
        idx_t exp = ((idx_t)1) << (cur - 1);

        /* Allocate array for p_next(x) of degree exp */
        seq_t *d_next = (seq_t *)malloc((exp + 1) * sizeof(seq_t));

        /* Set the coefficient of x^{exp} to 1 */
        d_next[exp] = (seq_t)1;

        /* Set lower coefficients as the negative of the current product P(x) */
        for (idx_t i = 0; i < exp; ++i) {
            d_next[i] = -(P[i]);            /* P has degree exp-1 */
        }

        /* Store the new row */
        row_poly[next] = d_next;
        row_len[next] = exp + 1;
        cumulative[next] = cumulative[cur] + row_len[next];
        rows_done = next;

        /* Compute new product P_next = P * d_next.
           new degree = P_deg + exp   (since deg d_next = exp).   */
        idx_t new_deg = P_deg + exp;
        seq_t *P_next = (seq_t *)calloc((size_t)new_deg + 1, sizeof(seq_t));

        for (idx_t i = 0; i <= P_deg; ++i) {
            for (idx_t j = 0; j <= exp; ++j) {
                P_next[i + j] += P[i] * d_next[j];
            }
        }

        /* Free old product P and update pointer */
        free(P);
        P = P_next;
        P_deg = new_deg;
    }
}

/* Main entry required by the judge. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* indices below offset are undefined */
    build_up_to(n);

    /* Determine in which row the term n lies. */
    idx_t r = 1;
    while (cumulative[r] < n) {
        ++r;
    }

    /* Offset within that row (0‑based). */
    idx_t offset = n - cumulative[r - 1] - 1;
    return row_poly[r][offset];
}
