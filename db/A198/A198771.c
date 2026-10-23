/*
 * A198771  a(n) = (11*5^n-1)/2.
 * https://oeis.org/A198771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*5^n - 1)/2; a(0) = 5 Recurrence: a(n) = 5*a(n-1) + 2,
 *            a(0) = 5
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow5 = pow_uint(5, n);
    seq_t res  = 11 * pow5;
    res -= 1;
    res /= 2;
    return res;
}
