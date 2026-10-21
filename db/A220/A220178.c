/*
 * A220178  Triangle where the g.f. for row n equals d^n/dx^n (1+x+x^2)^n / n!
 *          for n>=0, as read by rows.
 * https://oeis.org/A220178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle of coefficients * of
 *            the generating function row n = (d^n/dx^n)(1+x+x^2)^n / n! * row
 *            n has n+1 terms. a(0)=1; a(1)=1,2; a(2)=3,6,6; * this function
 *            computes a(n) for any non‑negative n
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 111, u64 n <= 388,
 *            u128 n <= 1348, big any n
 * Helpers:   binomial, find_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* For bool if needed */
#include "../utils/binomial.h"  /* binomial(idx_t n, idx_t k) returns seq_t */
#include "../utils/find_row_col.h"

/* Compute the coefficient a(row, col) of the triangle.
 * row >= 0, col in [0, row]. */
static seq_t triangle_coef(idx_t row, idx_t col) {
    idx_t sum_limit = (row + col) / 2;   /* floor((n+m)/2) */
    seq_t sum = 0;                      /* running sum over a */

    for (idx_t a = col; a <= sum_limit; ++a) {
        idx_t b = row + col - 2 * a;      /* b = n+m-2a, guaranteed >=0 */
        seq_t term = binomial(row, a);    /* C(n, a)   */
        term = term * binomial(row - a, b); /* C(n-a, b) */
        sum += term;
    }

    /* Result = C(n+m, m) * sum */
    return binomial(row + col, col) * sum;
}

/* Main function required by the build system. */
seq_t a(idx_t n) {
    idx_t row, col;
    find_row_col(n, &row, &col);
    return triangle_coef(row, col);
}
