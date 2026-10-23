/*
 * A109125  Column 7 of array illustrated in A089574 and related to A034261.
 * https://oeis.org/A109125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{9} (-c_k) * binom(n+1+8-k, 8) * where c_k are
 *            the coefficients of
 *            x*(-2-23*x+5*x^2+37*x^3-26*x^4-9*x^5+17*x^6-7*x^7+x^8) * i.e. c
 *            = [0, -2, -23, 5, 37, -26, -9, 17, -7, 1]. * This gives sequence
 *            A109125 with offset 0
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* shift index because the generating function starts at n+1 */
    idx_t m = n + 1;

    /* coefficients of -c_k = [0, 2, 23, -5, -37, 26, 9, -17, 7, -1] */
    static const int8_t  coeff_rev[10]   = {0,  2, 23, -5, -37,  26, 9, -17,  7, -1};
    static const seq_t   coeff_abs[10]   = {0,  2, 23, 5, 37, 26, 9, 17,  7,  1};

    seq_t sum = 0;

    for (idx_t k = 1; k <= 9; ++k)
    {
        idx_t top = m + 8 - k;
        if (top < 8)      /* binom(top,8) would be 0 */
            continue;

        seq_t bin = binom(top, (idx_t)8);

        if (coeff_rev[k] >= 0)
            sum += bin * coeff_abs[k];
        else
            sum -= bin * coeff_abs[k];
    }

    return sum;
}
