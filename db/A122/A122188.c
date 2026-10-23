/*
 * A122188  Triangle read by rows, formed from the coefficients of
 *          characteristic polynomials of the following sequence of matrices:
 *          2 X 2 {{0, 1}, {1, 1}}, 3 X 3 {{0, 1, 0}, {0, 0, 1}, {1, 1, 1}}, 4
 *          X 4 {{0, 1,0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}, {1, 1, 1, 1}}, 5 X 5
 *          {{0, 1, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0,...
 * https://oeis.org/A122188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of the polynomials
 *   B_d(x) = (-1)^d * (x^d - (1 + x + … + x^(d-1)))
 * a(n) is the (k)-th coefficient (0‑based) of B_d(x), where
 *   d = row‑1  (row 1 has degree 0),
 *   k = n - start_row_index,
 *   start_row_index = 1 + (row-1)*row/2.
 * The coefficient is +1 or –1; it equals (-1)^d for the last
 * coefficient of the row and (-1)^(d+1) for all others. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that the row starts at index start
       and contains indices [start, start+r-1] */
    idx_t r = 1;           /* row number, 1-based */
    idx_t start = 1;       /* first index of current row   */
    while (start + r - 1 < n) { /* n is not in this row yet */
        start += r;        /* move to start of next row   */
        r++;
    }
    idx_t col = n - start;        /* 0‑based column in the row  */
    idx_t d = r - 1;              /* degree of the polynomial   */
    int d_is_odd = d & 1;          /* parity of d                 */

    /* Determine sign based on position */
    if (col == d) {               /* last coefficient of the row*/
        return d_is_odd ? (seq_t)-1 : (seq_t)1;
    } else {                      /* all other coefficients    */
        return d_is_odd ? (seq_t)1 : (seq_t)-1;
    }
}
