/*
 * A006542  a(n) = binomial(n,3)*binomial(n-1,3)/4.
 * https://oeis.org/A006542
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^2*(n-2)^2*(n-3)/144
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 93, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   a006542_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006542_gcd.h"

seq_t a(idx_t n) {
    uint64_t f[6];
    uint64_t rem = 144;
    seq_t r = 1;
    int i;
    f[0] = (uint64_t)n;
    f[1] = (uint64_t)n - 1;
    f[2] = (uint64_t)n - 1;
    f[3] = (uint64_t)n - 2;
    f[4] = (uint64_t)n - 2;
    f[5] = (uint64_t)n - 3;
    for (i = 0; i < 6; i++) {
        uint64_t g = a006542_gcd(f[i], rem);
        f[i] /= g;
        rem /= g;
    }
    for (i = 0; i < 6; i++) {
        r *= (seq_t)f[i];
    }
    return r;
}
