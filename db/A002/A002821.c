/*
 * A002821  a(n) = nearest integer to n^(3/2).
 * https://oeis.org/A002821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n^(3/2) + 1/2) = m + [n^3 >= m^2 + m + 1], where m
 *            = floor(sqrt(n^3))
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 938 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
  if (n == 0) return 0;
  if (n < ((idx_t)1 << 40)) {
    unsigned __int128 N = (unsigned __int128)n * n * n;
    unsigned __int128 m = isqrt_u128(N);
    if (N >= m * m + m + 1) m++;
    return (seq_t)(uint64_t)m;
  } else {
    seq_t ns = (seq_t)n;
    seq_t N = ns * ns * ns;
    seq_t m = isqrt_seq(N);
    if (N >= m * m + m + 1) m++;
    return m;
  }
}
