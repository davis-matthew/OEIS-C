/*
 * A000605  Number of points of norm <= n in cubic lattice.
 * https://oeis.org/A000605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z) in Z^3 : x^2 + y^2 + z^2 <= n^2} =
 *            A117609(n^2); counted directly per x by sweeping y up and z down
 *            with two pointers
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1008, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a605_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a605_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t N2 = N * N;
    uint64_t total = 0;
    uint64_t x;
    for (x = 0; x <= N; x++) {
        uint64_t R = N2 - x * x;
        uint64_t z = a605_isqrt(R);
        uint64_t c = 2 * z + 1;
        uint64_t y;
        for (y = 1; y * y <= R; y++) {
            uint64_t rem = R - y * y;
            while (z * z > rem) z--;
            c += 2 * (2 * z + 1);
        }
        total += (x == 0) ? c : 2 * c;
    }
    return (seq_t)total;
}
