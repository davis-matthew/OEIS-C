/*
 * A005869  Theta series of b.c.c. lattice with respect to short edge.
 * https://oeis.org/A005869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r_3(8n+3)/4 = 2 * #{(x,y,z) positive odd integers :
 *            x^2+y^2+z^2 = 8n+3} (counted by direct search over x, y)
 * Range:     u8 n <= 720, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005869_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005869_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = 8 * (uint64_t)n + 3;
    uint64_t cnt = 0;
    uint64_t x, y;
    for (x = 1; x * x < N; x += 2) {
        for (y = 1; x * x + y * y < N; y += 2) {
            uint64_t rem = N - x * x - y * y;
            uint64_t z = a005869_isqrt(rem);
            if (z * z == rem && (z & 1)) cnt++;
        }
    }
    return (seq_t)(2 * cnt);
}
