/*
 * A060546  a(n) = 2^ceiling(n/2).
 * https://oeis.org/A060546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^ceil(n/2)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute ceil(n/2) without overflow */
    idx_t exp = n >> 1;
    if (n & 1) exp++;

    return pow2(exp);
}
