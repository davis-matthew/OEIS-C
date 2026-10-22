/*
 * A004531  Number of integer solutions to x^2 + 4 * y^2 = n.
 * https://oeis.org/A004531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with x^2 + 4*y^2 = n,
 *            counted by direct search over y
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004531_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004531_isqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    uint64_t y;
    for (y = 0; 4 * y * y <= (uint64_t)n; y++) {
        uint64_t rem = (uint64_t)n - 4 * y * y;
        uint64_t x = a004531_isqrt(rem);
        if (x * x == rem) {
            int xf = (x == 0) ? 1 : 2;
            int yf = (y == 0) ? 1 : 2;
            count += (seq_t)(xf * yf);
        }
    }
    return count;
}
