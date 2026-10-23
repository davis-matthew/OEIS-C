/*
 * A154117  Expansion of (1 - x + 3*x^2)/((1-x)*(1-2*x)).
 * https://oeis.org/A154117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n-1)-3 for n>=1, a(0)=1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n != 0)
    {
        /* n is unsigned, so n-1 is safe after the check */
        seq_t p = pow_uint(2, n - 1);  /* compute 2^(n-1) */
        return 5 * p - 3;
    }
    return (seq_t)1;
}
