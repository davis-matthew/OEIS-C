/*
 * A052689  Expansion of e.g.f. (1+x-x^2)/((1-x)*(1-x^2)).
 * https://oeis.org/A052689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*(2*n+5-(-1)^n)/4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* compute (2*n+5-(-1)^n)/4 as integer */
    seq_t factor_num;
    if ((n & 1) == 0) {          /* n even: (-1)^n = 1 */
        factor_num = (seq_t)(2*n + 4);   /* 2*n+5-1 = 2*n+4 */
    } else {                     /* n odd : (-1)^n = -1 */
        factor_num = (seq_t)(2*n + 6);   /* 2*n+5-(-1) = 2*n+6 */
    }
    seq_t factor = factor_num >> 2;        /* divide by 4 (exact) */

    return fact * factor;
}
