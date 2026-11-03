/*
 * A003512  A Beatty sequence: floor(n*(sqrt(3) + 2)).
 * https://oeis.org/A003512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n + floor(sqrt(3*n^2)) = floor(n*(sqrt(3)+2))
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8897 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 62)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 r = 2 * N + isqrt_u128(3 * N * N);
        return (seq_t)(uint64_t)r;
    }
    seq_t m = (seq_t)n;
    return 2 * m + isqrt_seq(3 * m * m);
}
