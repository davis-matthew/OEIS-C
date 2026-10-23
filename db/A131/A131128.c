/*
 * A131128  Binomial transform of [1, 1, 5, 1, 5, 1, 5, ...].
 * https://oeis.org/A131128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - 4 for n >= 1; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
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
    return (seq_t)3 * pow2(n) - (seq_t)4;
}
