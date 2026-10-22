/*
 * A001962  A Beatty sequence: floor(n * (sqrt(5) + 3)).
 * https://oeis.org/A001962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(sqrt(5)+3)) = 3*n + floor(sqrt(5*n^2))
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001962_isqrt64, a001962_isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001962_isqrt64.h"
#include "../utils/a001962_isqrt_seq.h"

seq_t a(idx_t n) {
    if (n < (idx_t)1900000000ULL) {
        uint64_t m = (uint64_t)n;
        uint64_t r = a001962_isqrt64(5 * m * m);
        return (seq_t)(3 * m + r);
    } else {
        seq_t m = (seq_t)n;
        return (seq_t)3 * m + a001962_isqrt_seq((seq_t)5 * m * m);
    }
}
