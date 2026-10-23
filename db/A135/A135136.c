/*
 * A135136  a(n) = floor(S2(n)/2) mod 2, where S2(n) is the binary weight of
 *          n.
 * https://oeis.org/A135136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(S2(n)/2) mod 2, where S2(n) is the binary weight of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned popcount_u64(uint64_t x) {
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    unsigned pc = popcount_u64((uint64_t)n);
    return (seq_t)((pc >> 1) & 1U);
}
