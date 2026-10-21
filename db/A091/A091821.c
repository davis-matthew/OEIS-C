/*
 * A091821  a(n) counts the one-bits in the binary representation of n when we
 *          cycle from left to right through the individual bits of n,
 *          restarting at the leftmost bit after the rightmost is reached and
 *          performing n bitwise steps in total.
 * https://oeis.org/A091821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts the ones seen after walking n steps over the
   binary representation of n, cycling the bits.  Given a number n, let
   len = bitlen(n), c = popcount(n) and p = n % len; then

   a(n) = (n / len) * c + popcount( n >> (len - p) )      (p > 0)
           (n / len) * c                                 (p = 0)
   -- This formula follows from the fact that each full cycle of
   len steps visits every bit once. */

#include <stdint.h>

static inline unsigned popcount_idx(uint64_t x)
{
    return __builtin_popcountll(x);
}

static inline unsigned bitlen_idx(uint64_t x)
{
    /* Number of bits needed to represent x (x>=1). */
    return x == 0 ? 0 : 64 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard */
    unsigned ones = popcount_idx(n);
    unsigned len  = bitlen_idx(n);
    idx_t full    = n / len;
    idx_t rem     = n % len;
    seq_t result  = (seq_t)full * (seq_t)ones;
    if (rem) {
        uint64_t leading = n >> (len - rem);
        result += (seq_t)popcount_idx(leading);
    }
    return result;
}
