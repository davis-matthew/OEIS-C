/*
 * A005585  5-dimensional pyramidal numbers: a(n) =
 *          n*(n+1)*(n+2)*(n+3)*(2n+3)/5!.
 * https://oeis.org/A005585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(2n+3)/5!
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005585_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005585_gcd_u64.h"

seq_t a(idx_t n) {
    uint64_t f[5];
    uint64_t d = 120;
    seq_t r = 1;
    int i;
    f[0] = (uint64_t)n;
    f[1] = (uint64_t)n + 1;
    f[2] = (uint64_t)n + 2;
    f[3] = (uint64_t)n + 3;
    f[4] = 2 * (uint64_t)n + 3;
    for (i = 0; i < 5 && d > 1; i++) {
        uint64_t g = a005585_gcd_u64(f[i], d);
        f[i] /= g;
        d /= g;
    }
    for (i = 0; i < 5; i++)
        r *= (seq_t)f[i];
    return r;
}
