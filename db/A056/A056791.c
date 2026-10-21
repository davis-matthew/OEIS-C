/*
 * A056791  Weight of binary expansion of n + length of binary expansion of n.
 * https://oeis.org/A056791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = popcount(n) + bitlen(n) with a(0)=1
 * Range:     u8 n <= 9658, u16 n <= 9658, u32 n <= 9658, u64 n <= 9658,
 *            u128 n <= 9658, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sequence value a(n) for any n >= 0.  The return type seq_t
   will be the appropriate integer type for the build (uint8_t ...,
   unsigned __int128, or an arbitrary‑precision class).  The calculation
   is performed in 64‑bit arithmetic which is sufficient because
   popcount(n) <= 64 and bitlen(n) <= 64, so the sum fits in 128 bits.
   When the build uses seq_t bigger than 128 bits, the cast automatically
   promotes the result. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 because bitlen(0)=1, popcount(0)=0 */
        return (seq_t)1;

    /* popcount via simple loop (portable, works for any compiler) */
    uint64_t w = 0, x = n;
    while (x) {
        w += x & 1;
        x >>= 1;
    }

    /* bitlen = floor(log2(n)) + 1.  For n > 0 we can use clz */
    unsigned len = 64 - __builtin_clzll(n);

    return (seq_t)(w + len);
}
