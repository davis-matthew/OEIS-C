/*
 * A005877  Theta series of cubic lattice with respect to square.
 * https://oeis.org/A005877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer triples (x,y,z) with (2x+1)^2 +
 *            (2y+1)^2 + (2z)^2 = 4n+2 (lattice points of Z^3 at squared
 *            distance n+1/2 from the square center (1/2,1/2,0)); a(n) =
 *            4*A045828(n)
 * Range:     u8 n <= 407, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a005877_isqrt, a005877_odd_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005877_isqrt.h"
#include "../utils/a005877_odd_pairs.h"

seq_t a(idx_t n) {
    uint64_t N = 4 * (uint64_t)n + 2;
    uint64_t total = 0;
    uint64_t z;
    for (z = 0; 4 * z * z < N; z++) {
        uint64_t c = a005877_odd_pairs(N - 4 * z * z) * 4;
        total += (z == 0) ? c : 2 * c;
    }
    seq_t res = 0;
    uint64_t i;
    /* build result in seq_t without assuming a constructor from uint64_t */
    for (i = 63; ; i--) {
        res = res * 2;
        if ((total >> i) & 1) res = res + 1;
        if (i == 0) break;
    }
    return res;
}
