/*
 * A101926  a(n) = 2^A101925(n).
 * https://oeis.org/A101926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+1 - popcount(n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* popcount for a 64‑bit integer */
    idx_t ones = __builtin_popcountll((uint64_t)n);
    /* exponent of the power of two */
    idx_t exp  = (2 * n + 1) - ones;
    return pow2(exp);
}
