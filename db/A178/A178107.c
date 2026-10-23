/*
 * A178107  Coefficient array for orthogonal polynomials
 *          P(n,x)=x*P(n-1,x)-(2*floor((n+2)/2)-3)*P(n-2,x),
 *          P(0,x)=1,P(1,x)=x.
 * https://oeis.org/A178107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 37, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of orthogonal polynomials P(n,x) given by
   P(0,x) = 1,
   P(1,x) = x,
   P(n,x) = x*P(n-1,x) - (2*floor((n+2)/2)-3)*P(n-2,x).
   The flattened array a(n) is defined by a(0)=1, a(1)=0, a(2)=1, a(3)=-1, … */

#include <stdlib.h>

/* Global storage for coefficient rows, built on demand */
static seq_t **rows            = NULL;      /* rows[i] points to array of i+1 coefficients */
static idx_t max_row_computed  = 0;         /* number of rows that have been built */

/* Build the next row of the coefficient triangle (row index i = max_row_computed) */
static void build_next_row(void)
{
    idx_t i = max_row_computed;   /* Index of row to build */

    if (i == 0) {
        rows = (seq_t**)realloc(rows, sizeof(seq_t*));
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        rows[0][0] = (seq_t)1;
    } else if (i == 1) {
        rows = (seq_t**)realloc(rows, 2 * sizeof(seq_t*));
        rows[1] = (seq_t*)malloc(2 * sizeof(seq_t));
        rows[1][0] = (seq_t)0;
        rows[1][1] = (seq_t)1;
    } else {
        /* Compute constant C = 2*floor((i+2)/2)-3 */
        idx_t half    = (i + 2) / 2;
        int   C_int   = (int)(2 * half - 3);
        seq_t C       = (seq_t)C_int;

        rows = (seq_t**)realloc(rows, (i + 1) * sizeof(seq_t*));
        rows[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));

        for (idx_t j = 0; j <= i; ++j) {
            seq_t a = (j > 0) ? rows[i - 1][j - 1] : (seq_t)0;
            seq_t b = (j <= i - 2) ? rows[i - 2][j] : (seq_t)0;
            rows[i][j] = a - C * b;
        }
    }
    ++max_row_computed;
}

/* Ensure that rows 0…r have been built */
static void ensure_rows(idx_t r)
{
    while (max_row_computed <= r) {
        build_next_row();
    }
}

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    idx_t t = 0;                    /* cumulative element count so far (sum of 1..r) */
    while (t + r + 1 <= n) {
        t += r + 1;
        ++r;
    }
    return r;
}

/* Return the flattened coefficient array a(n) */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);          /* row containing a(n) */
    idx_t t = r * (r + 1) / 2;      /* index of first element in row r */
    idx_t k = n - t;                /* column within the row */

    ensure_rows(r);

    return rows[r][k];
}
