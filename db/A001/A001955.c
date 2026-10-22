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
 * Helpers:   a001955_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001955_isqrt.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return N + a001955_isqrt(N * N / 11);
}
