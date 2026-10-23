/*
 * A175739  Triangle T(n,m) with the coefficient [x^m] of the polynomial
 *          x^(2*n) - x^(2*n - 1) - x^n - x + 1 in row n, column m, 1 <= m <=
 *          2*n. T(0,0) = 1.
 * https://oeis.org/A175739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) = coefficient of x^(k) in the polynomial P_r(x) = x^(2*r) -
 *            x^(2*r-1) - x^r - x + 1, where r = floor(sqrt(n)) and k = n -
 *            r^2
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* compute r = floor(sqrt(n)) using long double and adjust */
    uint64_t r = (uint64_t)sqrtl((long double)n);
    while (( (__int128)(r + 1) * (r + 1) ) <= n) ++r;
    while (( (__int128)r * r ) > n) --r;

    uint64_t offset = r * r;
    uint64_t k = n - offset;

    if (r == 0) {
        /* P_0(x) is defined as 1 */
        return (seq_t)1;
    }

    if (k == 0) {
        /* constant term */
        return (seq_t)1;
    }

    if (k == 1) {
        /* coefficient of x^1: -x, -x^r and possibly -x^(2r-1) if r==1 */
        if (r == 1) return (seq_t)(-3);
        else        return (seq_t)(-1);
    }

    if (k == r) {
        /* coefficient of x^r (not equal to 1 for r>1) */
        return (seq_t)(-1);
    }

    if (k == 2 * r - 1) {
        /* coefficient of x^(2r-1) */
        return (seq_t)(-1);
    }

    if (k == 2 * r) {
        /* leading coefficient */
        return (seq_t)1;
    }

    /* all other degrees have coefficient 0 */
    return (seq_t)0;
}
