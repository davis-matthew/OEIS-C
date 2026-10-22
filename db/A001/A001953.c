/*
 * A001953  a(n) = floor((n + 1/2) * sqrt(2)).
 * https://oeis.org/A001953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2*n*(n+1))) = floor(2*sqrt(A000217(n))), equal
 *            to floor((n + 1/2)*sqrt(2))
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001953_isqrt64, a001953_isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001953_isqrt64.h"
#include "../utils/a001953_isqrt_seq.h"

seq_t a(idx_t n) {
    if (n < ((uint64_t)1 << 30)) {
        uint64_t v = 2 * (uint64_t)n * ((uint64_t)n + 1);
        return (seq_t)a001953_isqrt64(v);
    }
    {
        seq_t m = (seq_t)n;
        seq_t v = 2 * m * (m + 1);
        return a001953_isqrt_seq(v);
    }
}
