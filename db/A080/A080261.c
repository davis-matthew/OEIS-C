/*
 * A080261  Simple involution of natural numbers: complement
 *          [binary_width(n)/2] least significant bits in the binary expansion
 *          of n.
 * https://oeis.org/A080261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR ((1 << floor(bitlen(n)/2)) - 1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complement floor(bitlen(n)/2) least significant bits of n */

#include <stdint.h>

static inline unsigned bitlen_uint64(uint64_t v)
{
    unsigned w = 0;
    while (v) {
        v >>= 1;
        ++w;
    }
    return w;
}

seq_t a(idx_t n)
{
    unsigned w = bitlen_uint64(n);
    unsigned k = w / 2;
    seq_t mask = (((seq_t)1) << k) - 1;
    return ((seq_t)n) ^ mask;
}
