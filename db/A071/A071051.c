/*
 * A071051  Number of 1's in n-th row of triangle in A071035.
 * https://oeis.org/A071051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(1+popcount(n)) if n+1 is a power of two (i.e. n =
 *            2^i-1) then a(n) = a(n) - 1. This matches the OEIS definition
 *            A071051
 * Range:     u8 n <= 190, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   popcnt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcnt_uint64.h"

seq_t a(idx_t n)
{
    /* Compute 2^(1 + popcount(n)).  Shift amount fits in 64 bits
       because popcount(n) ≤ 64 for uint64_t indices. */
    const unsigned int pc = popcnt_uint64((uint64_t)n);
    seq_t val = ((seq_t)1) << (1 + pc);

    /* Subtract 1 if n = 2^i - 1, i.e. n & (n+1) == 0. */
    if ((n & (n + 1)) == 0)
        val -= 1;

    return val;
}
