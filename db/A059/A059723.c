/*
 * A059723  Mean of the first 2^n nonnegative powers of 3, i.e., (1 + 3 + 3^2
 *          + ... + 3^(2^n-1))/2^n.
 * https://oeis.org/A059723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(2^n) - 1) / 2^(n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = 3;               /* start with 3^1 */
    for (idx_t i = 0; i < n; ++i) {
        val = val * val;        /* square: val becomes 3^(2^(i+1)) */
    }
    seq_t denom = pow2(n + 1);   /* 2^(n+1) */
    return (val - 1) / denom;
}
