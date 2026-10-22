/*
 * A004024  Theta series of b.c.c. lattice with respect to deep hole.
 * https://oeis.org/A004024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4 * #{(x,y,z) : x,y,z >= 1 odd, x^2 + 2*y^2 + 2*z^2 = 8*n
 *            + 5} (from 4*eta(32z)^4/eta(8z) = 4*Sum q^(x^2+2y^2+2z^2),
 *            exponents divided by 8)
 * Range:     u8 n <= 812, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

static int is_sq_u64(uint64_t v, uint64_t *root) {
    uint64_t r = a000037_isqrt(v);
    while (r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    *root = r;
    return r * r == v;
}

seq_t a(idx_t n) {
    uint64_t m = 8 * (uint64_t)n + 5;
    uint64_t count = 0;
    uint64_t x, y;
    for (x = 1; x * x + 4 <= m; x += 2) {
        uint64_t rem = m - x * x;
        if (rem & 1) continue;
        rem /= 2; /* y^2 + z^2 = rem */
        for (y = 1; y * y + 1 <= rem; y += 2) {
            uint64_t z2 = rem - y * y;
            uint64_t z;
            if (is_sq_u64(z2, &z) && (z & 1)) count++;
        }
    }
    return (seq_t)4 * (seq_t)count;
}
