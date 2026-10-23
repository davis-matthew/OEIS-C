/*
 * A161825  a(n) is the GCD of n and {the number made by reversing the order
 *          of the digits of n written in binary}.
 * https://oeis.org/A161825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = gcd(n, reverse_binary(n)) where reverse_binary(n) is
 *            obtained by reversing the order of the bits of n in binary and
 *            ignoring leading zeros
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t t = n;
    while (t > 0) {
        rev = (rev << 1) | ((seq_t)(t & 1));
        t >>= 1;
    }
    return gcd((seq_t)n, rev);
}
