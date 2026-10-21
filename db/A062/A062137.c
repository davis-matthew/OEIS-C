/*
 * A062137  Coefficient triangle of generalized Laguerre polynomials
 *          n!*L(n,3,x) (rising powers of x).
 * https://oeis.org/A062137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the flattening (row‑by‑row) of the coefficient triangle
 *            a(n,m) = ((-1)^m)*n!*binomial(n+3, n-m)/m!. The function
 *            computes the n‑th flattened element by finding the corresponding
 *            row and column, then evaluating the formula
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 55, i64 n <= 171, i128 n <= 496,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* count to find the row index (0‑based) that contains the n‑th element */
    double d = sqrt(1.0 + 8.0 * (double)n);
    uint64_t row = ((uint64_t)(d - 1.0)) / 2;          /* floor((sqrt(1+8n)-1)/2) */

    /* number of elements before this row: 0 + 1 + ... + (row-1) */
    uint64_t row_prefix = row * (row + 1) / 2;

    /* column index within the row */
    uint64_t col = n - row_prefix;   /* 0 <= col <= row */

    /* compute the coefficient a(row, col) */
    seq_t fact_row = factorial((idx_t)row);
    seq_t fact_col = factorial((idx_t)col);
    seq_t bin = binomial((idx_t)(row + 3), (idx_t)(row - col));

    seq_t res = fact_row * bin;
    res = res / fact_col;            /* division exact due to definition */

    if (col & 1)                     /* apply the (-1)^col sign */
        res = -res;

    return res;
}
