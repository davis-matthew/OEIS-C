/*
 * A049384  a(0) = 1, a(n+1) = (n+1)^a(n).
 * https://oeis.org/A049384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = (n+1)^a(n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t k = 1; k <= n; ++k) {
        val = big_pow((seq_t)k, val);
    }
    return val;
}
