/*
 * A238845  Prefix overlap between binary expansions of n and n+1.
 * https://oeis.org/A238845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0 a(n) = floor(log2(n+1)) - v2(n+1) +
 *            [is_power_of_two(n+1)] for n > 0 (where v2(x) is the exponent of
 *            the largest power of 2 dividing x)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline int floor_log2_idx(idx_t x) {
    int r = 0;
    while (x >>= 1) r++;
    return r;
}

static inline int trailing_zeros_idx(idx_t x) {
    int r = 0;
    while ((x & 1ULL) == 0) {
        r++;
        x >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* guard against overflow of n+1 for the theoretical maximum value */
    if (n == UINT64_MAX) return (seq_t)0;

    idx_t m = n + 1U;        /* m = n+1 > 0 */

    int log2  = floor_log2_idx(m);
    int v2    = trailing_zeros_idx(m);
    bool pow2 = (m & (m - 1)) == 0;

    seq_t result = (seq_t)(log2 - v2 + (int)pow2);
    return result;
}
