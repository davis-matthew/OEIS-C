/*
 * A165743  The greatest common divisor of n and 210.
 * https://oeis.org/A165743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = gcd(n,210)
 * Range:     u8 n <= 1563, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    return gcd((seq_t)n, (seq_t)210);
}
