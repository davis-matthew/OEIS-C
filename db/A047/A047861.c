/*
 * A047861  a(n) = T(4,n), array T given by A047858.
 * https://oeis.org/A047861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(n+8) - 3 for n ≥ 1, with a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = pow2(n - 1);
    val *= (seq_t)(n + 8U);
    val -= (seq_t)3;
    return val;
}
