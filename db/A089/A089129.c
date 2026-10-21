/*
 * A089129  Greatest common divisor of n^2 - 7 and n^2 + 7.
 * https://oeis.org/A089129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n+7, 14)
 * Range:     u8 n <= 325, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n + (seq_t)7;
    return gcd(val, (seq_t)14);
}
