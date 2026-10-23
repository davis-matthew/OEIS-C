/*
 * A115517  The mode of the bits of n (using 1 if bimodal).
 * https://oeis.org/A115517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 if the number of 0‑bits in the binary * representation
 *            of n exceeds the number of 1‑bits; otherwise a(n)=1. * (If n==0,
 *            a(0)=0.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Portable popcount for uint64_t */
static inline unsigned popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned c = 0;
    while (x) { c += (unsigned)(x & 1); x >>= 1; }
    return c;
#endif
}

/* Compute the bit length of a uint64_t (number of bits needed to represent it) */
static inline unsigned bitlen_u64(uint64_t x)
{
    if (x == 0) return 0;
#if defined(__GNUC__) || defined(__clang__)
    return 64U - __builtin_clzll(x);
#else
    unsigned len = 0;
    while (x) { len++; x >>= 1; }
    return len;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) defined as 0 */
    unsigned ones  = popcount_u64((uint64_t)n);
    unsigned bits  = bitlen_u64((uint64_t)n);
    unsigned zeros = bits - ones;
    return zeros > ones ? (seq_t)0 : (seq_t)1;
}
