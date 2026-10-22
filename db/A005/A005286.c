/*
 * A005286  a(n) = (n + 3)*(n^2 + 6*n + 2)/6.
 * https://oeis.org/A005286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n + 3)*(n^2 + 6*n + 2)/6 = binomial(n+4,3) - (n+3)
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 3;
    uint64_t x = m - 1, y = m, z = m + 1;
    int i;
    uint64_t f[3];
    seq_t r;
    f[0] = x; f[1] = y; f[2] = z;
    for (i = 0; i < 3; i++) {
        if (f[i] % 2 == 0) { f[i] /= 2; break; }
    }
    for (i = 0; i < 3; i++) {
        if (f[i] % 3 == 0) { f[i] /= 3; break; }
    }
    r = (seq_t)f[0];
    r *= (seq_t)f[1];
    r *= (seq_t)f[2];
    r -= (seq_t)m;
    return r;
}
