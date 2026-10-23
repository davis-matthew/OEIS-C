/*
 * A139698  Binomial transform of [1, 25, 25, 25, ...].
 * https://oeis.org/A139698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25 * 2^(n-1) - 24
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
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
    seq_t pow_val = pow2(n - 1);
    seq_t result = (seq_t)25 * pow_val;
    result -= (seq_t)24;
    return result;
}
