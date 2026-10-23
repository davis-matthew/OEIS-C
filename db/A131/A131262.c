/*
 * A131262  a(n) = least index k such that A130654(k) = n.
 * https://oeis.org/A131262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = 2^(2n) - floor(2^n/2) = 2^(2n) - 2^(n-1) (n > 0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = pow2(2*n);
    if (n > 0)
        res -= pow2(n-1);
    return res;
}
