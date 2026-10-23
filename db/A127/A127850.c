/*
 * A127850  a(n) = (2^n-1)*2^(n(n-1)/2)/(2^(n-1)).
 * https://oeis.org/A127850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n-1) * 2^(n(n-1)/2) / 2^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n == 0 is a special case: (2^0-1) = 0, so the result is 0. */
    if (n == 0) return 0;

    /* Compute 2^n and subtract 1. */
    seq_t pow_n = pow2(n);
    seq_t diff = pow_n - 1;

    /* exponent = (n-1)*(n-2)/2 */
    idx_t nm1 = n - 1;
    idx_t nm2 = nm1 > 0 ? nm1 - 1 : 0;      /* n-2, but avoid underflow  */
    idx_t exp = (nm1 * nm2) / 2;             /* product is always even */

    seq_t exp_pow = pow2(exp);

    return diff * exp_pow;
}
