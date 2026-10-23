/*
 * A037227  If n = 2^m*k, k odd, then a(n) = 2*m+1.
 * https://oeis.org/A037227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult, changed
 * Formula:   a(n) = 2*m+1 where n = 2^m * k, k odd (i.e. a(n) = 1 + 2*ctz(n))
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count trailing zeros of n. */
    unsigned m;
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang provide optimized builtin, undefined for n == 0 but n >= 1 here. */
    m = __builtin_ctzll((uint64_t)n);
#else
    /* Portable (fall-back) implementation. */
    m = 0;
    uint64_t tmp = (uint64_t)n;
    while ((tmp & 1ULL) == 0) {
        ++m;
        tmp >>= 1;
    }
#endif
    return (seq_t)1 + (seq_t)2 * (seq_t)m;
}
