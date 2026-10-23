/*
 * A011371  a(n) = n minus (number of 1's in binary expansion of n). Also
 *          highest power of 2 dividing n!.
 * https://oeis.org/A011371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n minus number of 1's in the binary expansion of n
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n - popcount(n) */

#include <stdint.h>

static inline unsigned int popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int cnt = 0;
    while (x) {
        cnt += x & 1U;
        x >>= 1;
    }
    return cnt;
#endif
}

seq_t a(idx_t n)
{
    /* Compute n minus the number of 1-bits of n */
    unsigned int pc = popcount_u64((uint64_t)n);
    return (seq_t)n - (seq_t)pc;
}
