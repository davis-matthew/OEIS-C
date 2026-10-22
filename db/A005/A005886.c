/*
 * A005886  Theta series of f.c.c. lattice with respect to tetrahedral hole.
 * https://oeis.org/A005886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * A008443(n), where A008443(n) is the number of ordered
 *            triples (x,y,z) >= 0 with T(x)+T(y)+T(z) = n, T(k) = k*(k+1)/2;
 *            counted by searching over x and y
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005886_is_tri, a005886_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005886_is_tri.h"
#include "../utils/a005886_isqrt.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t cnt = 0;
    uint64_t x, y;
    for (x = 0; x * (x + 1) / 2 <= m; x++) {
        uint64_t tx = x * (x + 1) / 2;
        for (y = 0; tx + y * (y + 1) / 2 <= m; y++) {
            uint64_t r = m - tx - y * (y + 1) / 2;
            if (a005886_is_tri(r)) cnt++;
        }
    }
    return (seq_t)cnt * 4;
}
