/*
 * A191272  Expansion of x*(4+5*x)/( (1-4*x)*(1 + x + x^2) ).
 * https://oeis.org/A191272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - (n mod 3 == 0 ? 1 : n mod 3 == 2 ? -1 : 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t val = pow4(n);

    /* periodic correction: 1,0,-1 for n mod 3 = 0,1,2 */
    idx_t m = n % 3;
    if (m == 0)
        val -= 1;           /* subtract 1 */
    else if (m == 2)
        val += 1;           /* subtract -1, i.e., add 1 */

    return val;
}
