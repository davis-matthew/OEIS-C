/*
 * A000910  a(n) = 5*binomial(n, 6).
 * https://oeis.org/A000910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*binomial(n, 6) = 5*n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)/720
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a910_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a910_gcd.h"

seq_t a(idx_t n) {
    uint64_t f[6];
    uint64_t d = 720;
    seq_t r = 1;
    int i;
    if (n < 6) return 0;
    for (i = 0; i < 6; i++) f[i] = (uint64_t)(n - (idx_t)i);
    for (i = 0; i < 6; i++) {
        uint64_t g = a910_gcd(f[i], d);
        f[i] /= g;
        d /= g;
    }
    for (i = 0; i < 6; i++) r *= (seq_t)f[i];
    return r * 5;
}
