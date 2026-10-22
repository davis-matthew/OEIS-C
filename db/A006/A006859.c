/*
 * A006859  From paths in the plane.
 * https://oeis.org/A006859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)^2*(n+4)*(2*n+3)*(2*n+5)*(2*n+7)/15120
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 24, u64 n <= 317,
 *            u128 n <= 10000, big any n
 * Helpers:   a006859_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006859_gcd.h"

seq_t a(idx_t n) {
    uint64_t f[9];
    uint64_t divisor = 15120;
    seq_t result = 1;
    int i;

    f[0] = (uint64_t)n + 1;
    f[1] = (uint64_t)n + 2;
    f[2] = (uint64_t)n + 2;
    f[3] = (uint64_t)n + 3;
    f[4] = (uint64_t)n + 3;
    f[5] = (uint64_t)n + 4;
    f[6] = 2 * (uint64_t)n + 3;
    f[7] = 2 * (uint64_t)n + 5;
    f[8] = 2 * (uint64_t)n + 7;

    /* cancel the denominator exactly against the factors first */
    for (i = 0; i < 9; i++) {
        uint64_t g = a006859_gcd(f[i], divisor);
        f[i] /= g;
        divisor /= g;
    }

    for (i = 0; i < 9; i++)
        result *= (seq_t)f[i];

    return result;
}
