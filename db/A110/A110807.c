/*
 * A110807  n times largest n-digit number.
 * https://oeis.org/A110807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*(10^n - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t ten_pow = pow10(n);      /* 10^n */
    seq_t res = ten_pow - (seq_t)1;
    res *= (seq_t)n;
    return res;
}
