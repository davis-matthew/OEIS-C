/*
 * A005929  Theta series of hexagonal net with respect to midpoint of edge.
 * https://oeis.org/A005929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer pairs (x,y) with x^2 + 3*y^2 = n if n
 *            == 1 (mod 6), else 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    seq_t cnt = 0;
    uint64_t y;
    if (n % 6 != 1) return cnt;
    for (y = 0; 3 * y * y <= n; y++) {
        uint64_t r = n - 3 * y * y;
        uint64_t x = a000037_isqrt(r);
        if (x * x == r) {
            /* x != 0 since n is not divisible by 3 */
            cnt += (y == 0) ? 2 : 4;
        }
    }
    return cnt;
}
