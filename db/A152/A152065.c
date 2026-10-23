/*
 * A152065  A triangular sequence of polynomial coefficients: p(x,n)=If[n ==
 *          0, x^n - x^Floor[(n - 1)/2]*Sum[x^m, {m, 0, n - Floor[(n - 1)/2] -
 *          1}] + 1/x, x^n - x^Floor[(n - 1)/2]*Sum[x^m, {m, 0, n - Floor[(n -
 *          1)/2] - 1}] + 1].
 * https://oeis.org/A152065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A152065
 * a(n) is the n-th element of the triangle formed by the
 * coefficients of the polynomials
 *    p(x,n) = x^n – x^⌊(n‑1)/2⌋ · Σ_{m=0}^{n–⌊(n‑1)/2⌋–1} x^m + 1
 * with the trailing +1 (or +1/x when n==0).  The triangle is
 * flattened in row‑major order (row 0 first, then row 1, …).
 * It can be computed using only index arithmetic and a few
 * conditional tests.  The result is always –1, 0 or 1.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that
     *   r*(r+1)/2 ≤ n < (r+1)*(r+2)/2          (0‑based)
     */
    idx_t row = 0;
    idx_t cum = 0;                     /* cumulative count before this row */

    while (n >= cum + row + 1) {       /* advance to next row if needed */
        cum += row + 1;                /* total terms in rows 0..row */
        row++;
    }

    idx_t col = n - cum;               /* column index within the row (0‑based) */

    /* Row 0 is the constant polynomial */
    if (row == 0) {
        return (seq_t)1;
    }

    /* For rows n ≥ 1 the coefficients follow:
     *   col == row          → +1
     *   col == 0            →  1 if floor((row‑1)/2) > 0, else 0
     *   0 < col < floor((row‑1)/2)   → 0
     *   floor((row‑1)/2) ≤ col < row   → –1
     */
    if (col == row) {
        return (seq_t)1;
    }

    idx_t k = (row - 1) / 2;           /* floor((row‑1)/2) */

    if (col == 0) {
        return (k == 0) ? (seq_t)0 : (seq_t)1;
    }

    if (col < k) {
        return (seq_t)0;
    }

    /* remaining case: k ≤ col < row */
    return (seq_t)-1;
}
