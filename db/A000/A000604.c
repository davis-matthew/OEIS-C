/*
 * A000604  Number of nonnegative solutions to x^2 + y^2 + z^2 <= n^2.
 * https://oeis.org/A000604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z) >= 0 : x^2 + y^2 + z^2 <= n^2}; counted
 *            directly: for each x, a two-pointer sweep over y (z decreasing)
 *            counts the lattice points in the quarter disk y^2 + z^2 <= n^2 -
 *            x^2
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   a604_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a604_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t N2 = N * N;
    seq_t total = 0;
    uint64_t x;

    for (x = 0; x <= N; x++) {
        uint64_t R = N2 - x * x;
        uint64_t z = a604_isqrt(R);
        uint64_t c = 0;
        uint64_t y;
        for (y = 0; y * y <= R; y++) {
            uint64_t rem = R - y * y;
            while (z * z > rem) z--;
            c += z + 1;
        }
        total += (seq_t)c;
    }
    return total;
}
