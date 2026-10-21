/*
 * A073138  Largest number having in its binary representation the same number
 *          of 0's and 1's as n.
 * https://oeis.org/A073138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, look, easy
 * Formula:   a(n) = (1 << bitlen(n)) - (1 << (bitlen(n) - bitcount(n)))
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest integer that has, in binary, the same number of
   0's and 1's as the input n.  The function works for any n >= 0.
   It uses only bitwise arithmetic and loops, compatible with all
   integer sizes supported by seq_t and idx_t. */

#include <stdint.h>

static inline unsigned popcnt_idx(idx_t x)
{
    unsigned c = 0;
    while (x)
    {
        x &= x - 1; /* clear lowest set bit */
        ++c;
    }
    return c;
}

static inline unsigned bitlen_idx(idx_t x)
{
    unsigned l = 0;
    while (x)
    {
        x >>= 1;
        ++l;
    }
    return l;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* number of bits needed to represent n (d) */
    unsigned d = bitlen_idx(n);
    /* number of 1's in n */
    unsigned ones = popcnt_idx(n);
    /* number of 0's in n */
    unsigned zeros = d - ones;

    /* (1 << d)  is 2^d, shift left by 'd' bits on a seq_t value. */
    seq_t high = ((seq_t)1) << d;
    /* (1 << zeros)  is 2^zeros */
    seq_t low = ((seq_t)1) << zeros;

    return high - low;
}
