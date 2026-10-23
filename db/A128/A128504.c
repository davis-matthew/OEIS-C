/*
 * A128504  Row sums of array A128503 (second convolution of Chebyshev's
 *          S(n,x) = U(n,x/2) polynomials).
 * https://oeis.org/A128504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) =
 *            (floor(n/3)+1)*(floor(n/3)-floor((n-1)/3)+(3/2)*(floor(n/3)+2)*(3*floor((n+1)/3)-n))*(-1)^n
 * Range:     i8 n <= 24, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   = (q+1)*(-1)^n           if n%3==0 */
/*   = -(q+1)*(q+2)*3/2*(-1)^n if n%3==1 */
/*   = (q+1)*(q+2)*3/2*(-1)^n   if n%3==2 */
/*   where q = floor(n/3). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* parity sign */
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;

    /* q = floor(n/3) */
    seq_t q = (seq_t)(n / 3);
    int r = (int)(n % 3);   /* remainder 0,1,2 */

    if (r == 0) {
        /* a(n) = (q+1) * sign */
        return (q + 1) * sign;
    } else {
        /* compute (q+1)*(q+2)*3/2 */
        seq_t prod = (q + 1) * (q + 2);   /* product of two consecutive integers, guaranteed even */
        seq_t half = prod / 2;           /* integer division, exact */
        seq_t term = half * 3;           /* (q+1)*(q+2)*3/2 */

        if (r == 1) {
            /* a(n) = -term * sign */
            return -term * sign;
        } else {
            /* r == 2 */
            /* a(n) = term * sign */
            return term * sign;
        }
    }
}
