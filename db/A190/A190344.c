/*
 * A190344  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(5/2), t=sqrt(2/5).
 * https://oeis.org/A190344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*sqrt(5/2)] + [n*sqrt(2/5)]
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t n_sq = N * N;
    seq_t temp = n_sq * (seq_t)10;
    seq_t t = floor_sqrt(temp);
    return N + t / (seq_t)2 + t / (seq_t)5;
}
