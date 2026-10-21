/*
 * A200678  Partial sums of A200675.
 * https://oeis.org/A200678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(2^floor(n/4)-1) + (n mod 4)*2^floor(n/4)
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t m = n / 4;
    idx_t r = n % 4;
    seq_t p = pow2(m);                 /* 2^m */
    seq_t res = 4 * (p - 1);           /* 4*(2^m - 1) */
    res += (seq_t)r * p;               /* + (n mod 4)*2^m */
    return res;
}
