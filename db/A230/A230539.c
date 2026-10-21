/*
 * A230539  a(n) = 3*n*2^(3*n-1).
 * https://oeis.org/A230539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*2^(3*n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t exp = 3*n - 1;
    seq_t power = pow2(exp);
    seq_t mult  = (seq_t)3 * (seq_t)n;
    return mult * power;
}
