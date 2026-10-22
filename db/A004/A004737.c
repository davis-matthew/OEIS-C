/*
 * A004737  Concatenation of sequences (1,2,...,n-1,n,n-1,...,1) for n >= 1.
 * https://oeis.org/A004737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, tabf
 * Formula:   Let b(n) = floor(sqrt(n-1)). Then a(n) = min(n - b(n)^2,
 *            (b(n)+1)^2 - n + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t b = isqrt_u64(n - 1);
    uint64_t diff = n - b * b;
    uint64_t inv = 2 * b + 2 - diff;
    uint64_t m = diff < inv ? diff : inv;
    return (seq_t)m;
}
