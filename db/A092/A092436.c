/*
 * A092436  a(n) = 1/2 + (-1)^n*(1/2 - A010060(floor(n/2))).
 * https://oeis.org/A092436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - A010060(n) where A010060(n) is the Thue-Morse
 *            sequence (parity of popcount of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8585 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Parity of the number of set bits in n (0 or 1). */
static inline seq_t parity64(idx_t n)
{
    uint64_t x = (uint64_t)n;
    /* XOR the halves repeatedly to mix all bits into the lowest bit. */
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (seq_t)(x & 1);
}

/* a(n) = 1 - parity(n), defined for n >= 1. */
seq_t a(idx_t n)
{
    return (seq_t)(1 - parity64(n));
}
