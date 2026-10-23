/*
 * A143114  a(n) = the smallest positive integer that is coprime to n and has
 *          the same number of (nonleading) 0's in its binary representation
 *          as n has in binary.
 * https://oeis.org/A143114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest positive integer coprime to n that has the
 *            same number of nonleading 0's in its binary representation as n
 * Range:     u8 n <= 127, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

static unsigned count_zero_bits(seq_t x)
{
    unsigned zeros = 0;
    while (x) {
        if ((x & 1) == 0)
            zeros++;
        x >>= 1;
    }
    return zeros;
}

seq_t a(idx_t n)
{
    /* zero count for n (nonleading zeros) */
    unsigned zero_n = count_zero_bits((seq_t)n);

    seq_t m = 1;
    while (true) {
        if (count_zero_bits(m) == zero_n && gcd((seq_t)n, m) == 1)
            return m;
        m++;
    }
}
