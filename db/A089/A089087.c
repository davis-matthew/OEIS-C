/*
 * A089087  Triangular array of coefficients multiplied by n! of polynomials
 *          in e. These give the expected number of trials needed for the sum
 *          of uniform random variables from the interval [0,1] to exceed n+1.
 * https://oeis.org/A089087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 28, i32 n <= 68, i64 n <= 174,
 *            i128 n <= 471, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A089087
 *
 *  Formula: a(n) is the flattened entry of the triangular array
 *           T(r, j) = (-1)^j * r! * (r+1-j)^j / j!
 *           where r = floor((sqrt(8*n+1)-1)/2) and
 *           j = n - r*(r+1)/2.
 *
 *  T(r, j) is the j-th coefficient (starting from 0) of the r-th row
 *  of the triangle whose rows are multiplied by r! and represent
 *  the expected number of uniform(0,1) trials needed for the
 *  sum to exceed r+1.
 *
 *  This implementation uses only integer arithmetic and works for
 *  any integer type seq_t supplied by the build environment.
 */

#include <stddef.h>  /* for idx_t, seq_t types */
#include <math.h>    /* for sqrtl, used only in row calculation */

seq_t a(idx_t n)
{
    /* Find row r: largest r such that r*(r+1)/2 <= n   */
    idx_t r = (idx_t)((sqrtl((long double)8*n+1) - 1) / 2);

    /* Compute index j within that row */
    idx_t j = n - r*(r+1)/2;

    /* Term: r! / j!  (product of integers j+1 .. r) */
    seq_t term = 1;
    for (idx_t i = j + 1; i <= r; ++i) {
        term *= (seq_t)i;          /* multiply by i */
    }

    /* Multiply by (r+1-j)^j */
    seq_t base = (seq_t)(r + 1 - j);
    seq_t power = 1;
    for (idx_t k = 0; k < j; ++k) {
        power *= base;
    }
    term *= power;

    /* Apply sign (-1)^j */
    if (j & 1) {
        term = -term;
    }

    return term;
}
