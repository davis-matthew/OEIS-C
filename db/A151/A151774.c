/*
 * A151774  Characteristic function of numbers with binary weight 2 (A018900).
 * https://oeis.org/A151774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the binary representation of n has exactly two
 *            1-bits, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int popcount_idx(idx_t x) {
    int c = 0;
    for (; x; x >>= 1)
        c += (int)(x & 1);
    return c;
}

seq_t a(idx_t n) {
    return (seq_t)(popcount_idx(n) == 2);
}
