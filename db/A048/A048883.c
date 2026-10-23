/*
 * A048883  a(n) = 3^wt(n), where wt(n) = A000120(n).
 * https://oeis.org/A048883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, hear
 * Formula:   a(n) = 3^wt(n), where wt(n) is the number of 1 bits in the
 *            binary expansion of n (A000120)
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

static inline unsigned int popcount_u64(idx_t x) {
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    unsigned int k = popcount_u64(n);
    return pow3(k);
}
