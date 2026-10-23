/*
 * A177107  a(n) = 3*(10^n-7).
 * https://oeis.org/A177107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(10^n-7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 10^n */
    seq_t t = pow10(n);

    /* subtract 7 and multiply by 3 */
    t -= (seq_t)7;
    t *= (seq_t)3;

    return t;
}
