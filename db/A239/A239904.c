/*
 * A239904  a(n) = n - wt(n) + (number of times 11 appears in binary expansion
 *          of n).
 * https://oeis.org/A239904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - wt(n) + A014081(n) * where wt(n) = number of 1-bits
 *            in the binary expansion of n * A014081(n) = number of
 *            overlapping occurrences of "11" in binary(n)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* portable popcount for 64‑bit values */
static inline unsigned int popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) {
        c += x & 1u;
        x >>= 1;
    }
    return c;
#endif
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* convert integer index to seq_t */
    seq_t sn = (seq_t)n;

    /* number of 1-bits in n */
    unsigned int ones = popcount64((uint64_t)n);

    /* number of times "11" appears (overlapping) */
    uint64_t pair_mask = ((uint64_t)n) & (((uint64_t)n) >> 1);
    unsigned int twice = popcount64(pair_mask);

    /* a(n) = n - ones + twice */
    return sn - (seq_t)ones + (seq_t)twice;
}
