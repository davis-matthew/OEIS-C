/*
 * A210881  Triangular array U(n,k) of coefficients of polynomials defined in
 *          Comments.
 * https://oeis.org/A210881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficients of the polynomials u(n,x) arranged
 *            row‑major. u(1) = 1 u(2,x) = 3x + 1 u(n,x) = x*u(n‑1,x) +
 *            x²*u(n‑2,x) + n*(x+1) (n ≥ 3) The array U(n,k) consists of the
 *            coefficients of u(n,x) in order (constant, x, x², …). The
 *            sequence a(n) is obtained by writing the rows one after
 *            another...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element. */
    idx_t r = 1;
    /* r*(r+1)/2 is the total number of elements up to row r. */
    while (((r * (r + 1)) / 2) < n) r++;

    /* Compute row r if it hasn't been computed yet.  We keep all
       previously computed rows in a static array of pointers. */
    static seq_t **rows = NULL;      /* rows[i] points to the i‑th row (1‑based) */
    static idx_t rows_size = 0;      /* highest row stored */

    if (rows == NULL) {
        /* Allocate space for rows 0 through 2 (row 0 unused). */
        rows_size = 2;
        rows = (seq_t **)calloc(rows_size + 1, sizeof(seq_t *));
        /* row 1: [1] */
        rows[1] = (seq_t *)malloc(sizeof(seq_t) * 1);
        rows[1][0] = (seq_t)1;
        /* row 2: [1, 3] */
        rows[2] = (seq_t *)malloc(sizeof(seq_t) * 2);
        rows[2][0] = (seq_t)1;
        rows[2][1] = (seq_t)3;
    } else if (r > rows_size) {
        /* Grow the rows array to hold up to row r. */
        rows = (seq_t **)realloc(rows, sizeof(seq_t *) * (r + 1));
        for (idx_t i = rows_size + 1; i <= r; ++i) {
            /* Each row i has i coefficients.  Use calloc to zero‑initialize. */
            seq_t *row = (seq_t *)calloc(i, sizeof(seq_t));
            if (i >= 3) {
                /* Contribution from x * u(i-1,x) */
                for (idx_t j = 1; j < i; ++j)
                    row[j] += rows[i - 1][j - 1];
                /* Contribution from x² * u(i-2,x) */
                for (idx_t j = 2; j < i; ++j)
                    row[j] += rows[i - 2][j - 2];
                /* Add the term n*(x+1): constant and x coefficient both += i */
                row[0] += (seq_t)i;
                row[1] += (seq_t)i;
            }
            rows[i] = row;
        }
        rows_size = r;
    }

    /* Position of n within row r. */
    idx_t prev_elements = ((r - 1) * r) / 2;   /* elements before row r */
    idx_t k = n - prev_elements;               /* 1‑based index within row r */

    return rows[r][k - 1];
}
