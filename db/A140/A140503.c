/*
 * A140503  Triangle T(d,n) read by rows, the n-th term of the d-th
 *          differences of the Jacobsthal sequence A001045.
 * https://oeis.org/A140503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = ( 2^col - 2^row * (-1)^(row+col) ) / 3 where row =
 *            smallest integer with row*(row+1)/2 ≥ n, col = n - (row-1)*row/2
 *            - 1 (OEIS A140503)
 * Range:     i8 n <= 21, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row `d` and the column `col` such that
       the first `d-1` rows contain (d-1)d/2 elements,
       and the flat index `n` lies in row `d`. */
    idx_t d = 1;          /* current row */
    idx_t sum = 0;        /* number of elements in all previous rows */

    while (sum + d < n) {
        sum += d;         /* elements in row d added */
        d++;              /* move to next row */
    }

    /* Column within row `d` (0‑based) */
    idx_t col = n - sum - 1;

    /* Compute the term using the closed formula */
    seq_t pow2_d  = pow2(d);
    seq_t pow2_col = pow2(col);

    int  parity = ((d + col) & 1) ? -1 : 1;           /* (-1)^(d+col) */
    seq_t term = pow2_col - pow2_d * (seq_t)parity;   /* numerator */
    term = term / 3;                                 /* divide by 3 */

    return term;
}
