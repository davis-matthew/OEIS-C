/*
 * A130104  Expansion of x(1-3x+5x^2-2x^3)/((1-x)^3*(1-2x)).
 * https://oeis.org/A130104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+1) - C(n+2, 2) - 0^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(n+1) */
    seq_t res = pow2(n + 1);

    /* C(n+2, 2) = (n+2)*(n+1)/2 */
    seq_t bin = ((seq_t)(n + 2) * (n + 1)) / 2;

    res -= bin;

    /* subtract 0^n : 0^0 = 1, 0^n = 0 for n > 0 */
    if (n == 0)
        res -= 1;

    return res;
}
