/*
 * A099549  Odd part of n modulo 8.
 * https://oeis.org/A099549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part of n modulo 8 * a(n) = (n >> v2(n)) & 7 * where
 *            v2(n) is the number of trailing zero bits in n (i.e., the
 *            exponent of 2 dividing n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 according to the sequence offset */
    idx_t t = n;
    /* count trailing zeros (powers of two) */
    while ((t & 1) == 0) {
        t >>= 1;
    }
    /* odd part modulo 8 is its last 3 bits */
    return (seq_t)(t & 7);
}
