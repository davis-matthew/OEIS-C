/*
 * A028353  Coefficient of x^(2*n+1) in arctanh(x)/sqrt(1-x^2), multiplied by
 *          (2*n+1)!.
 * https://oeis.org/A028353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=5; a(n) = (8*n*n-4*n+1)*a(n-1) -
 *            4*(n-1)*(n-1)*(2*n-1)*(2*n-1)*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)5;   /* a(1) */
    seq_t a_cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii          = (seq_t)i;
        seq_t coeff       = (seq_t)8 * ii * ii - (seq_t)4 * ii + (seq_t)1;
        seq_t term1       = coeff * a_prev1;

        seq_t im1         = (seq_t)(i - 1);
        seq_t twopi_m1    = (seq_t)(2 * i - 1);
        seq_t term2       = (seq_t)4 * im1 * im1 * twopi_m1 * twopi_m1 * a_prev2;

        a_cur = term1 - term2;

        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
