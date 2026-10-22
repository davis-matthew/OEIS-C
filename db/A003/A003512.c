/*
 * A003512  A Beatty sequence: floor(n*(sqrt(3) + 2)).
 * https://oeis.org/A003512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n + floor(sqrt(3*n^2)) = floor(n*(sqrt(3)+2))
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a003512_isqrt128, a003512_isqrt_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a003512_isqrt128.h"
#include "../utils/a003512_isqrt_big.h"

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 62)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 r = 2 * N + a003512_isqrt128(3 * N * N);
        return (seq_t)(uint64_t)r;
    }
    seq_t m = (seq_t)n;
    return 2 * m + a003512_isqrt_big(3 * m * m);
}
