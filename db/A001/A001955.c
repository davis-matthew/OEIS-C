/*
 * A001955  Beatty sequence of 1 + 1/sqrt(11).
 * https://oeis.org/A001955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1 + 1/sqrt(11))) = n +
 *            floor(sqrt(floor(n^2/11)))
 * Range:     u8 n <= 53, u16 n <= 1802, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 8483 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return N + isqrt_seq(N * N / 11);
}
