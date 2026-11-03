/*
 * A001962  A Beatty sequence: floor(n * (sqrt(5) + 3)).
 * https://oeis.org/A001962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(sqrt(5)+3)) = 3*n + floor(sqrt(5*n^2))
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9954 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    if (n < (idx_t)1900000000ULL) {
        uint64_t m = (uint64_t)n;
        uint64_t r = isqrt_u64(5 * m * m);
        return (seq_t)(3 * m + r);
    } else {
        seq_t m = (seq_t)n;
        return (seq_t)3 * m + isqrt_seq((seq_t)5 * m * m);
    }
}
