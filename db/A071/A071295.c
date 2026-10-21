/*
 * A071295  Product of numbers of 0's and 1's in binary representation of n.
 * https://oeis.org/A071295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   a(n) = A023416(n) * A000120(n) (product of the numbers of 0’s
 *            and 1’s in the binary representation of n)
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The following function computes the number of set bits (ones) in n.
   __builtin_popcountll is available in GCC/Clang and returns an unsigned
   int count of 1-bits in the 64‑bit argument. */
static unsigned int popcount64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return (x == 0) ? 0 : __builtin_popcountll(x);
#else
    /* Fallback: simple loop, should be very fast for 64‑bit inputs. */
    unsigned int c = 0;
    while (x) { c += (unsigned int)(x & 1); x >>= 1; }
    return c;
#endif
}

/* The following function returns the number of bits needed to represent
   a positive integer in binary (floor(log2(x)) + 1). For x==0 the length
   is defined as 1, because "0" is represented by a single bit. */
static unsigned int bitlen64(uint64_t x) {
    if (x == 0) return 1;
#if defined(__GNUC__) || defined(__clang__)
    return 64 - __builtin_clzll(x);
#else
    unsigned int len = 0;
    while (x) { len++; x >>= 1; }
    return len;
#endif
}

/* Main function required by the test harness.  seq_t and idx_t are typedefs
   defined elsewhere in the build.  No static keyword, no extra verbosity. */
seq_t a(idx_t n)
{
    /* Count the number of 1's in the binary representation of n. */
    unsigned int ones = popcount64((uint64_t)n);

    /* Determine the bit length of n.  For n==0 we want length 1. */
    unsigned int bits = bitlen64((uint64_t)n);

    /* Number of 0's is 'bits' minus 'ones'. */
    unsigned int zeros = bits - ones;

    /* Return the product as seq_t.  Intermediate values fit in 64‑bit
       unsigned int, so the cast is safe.  The multiplication may overflow
       if the result exceeds the width of seq_t; that is acceptable
       because the spec only requires exactness when the value fits. */
    return (seq_t)ones * (seq_t)zeros;
}
