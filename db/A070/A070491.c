/*
 * A070491  a(n) = n^3 mod 29.
 * https://oeis.org/A070491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 29
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    unsigned long long r = (unsigned long long)(n % 29);
    unsigned long long sq = (r * r) % 29;
    unsigned long long cube = (r * sq) % 29;
    return (seq_t)cube;
}
