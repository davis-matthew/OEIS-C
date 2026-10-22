/*
 * A005793  Number of O_1^{2+}(Z)-orbits of Lorentzian modular group.
 * https://oeis.org/A005793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (z,x,y) with n = z^2 - x^2 - y^2, z > x >= y >=
 *            0, z >= x+y (direct search)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

static uint64_t isqrt_u64(uint64_t v) {
    uint64_t r = a000037_isqrt(v);
    while (r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t count = 0;
    /* y = 0: z >= x+1 forces N >= 2x+1; y >= 1: z >= x+y forces N >= 2xy */
    for (uint64_t x = 0; 2 * x <= N; x++) {
        uint64_t max_y = (x == 0) ? 0 : N / (2 * x);
        if (max_y > x) max_y = x;
        for (uint64_t y = 0; y <= max_y; y++) {
            uint64_t zsq = N + x * x + y * y;
            uint64_t z = isqrt_u64(zsq);
            if (z * z == zsq && z > x && z >= x + y) count++;
        }
    }
    return (seq_t)count;
}
