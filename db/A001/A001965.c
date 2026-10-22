/*
 * A001965  u-pile count for the 4-Wythoff game with i=2.
 * https://oeis.org/A001965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n+1/2)*(sqrt(5)-1) ) = floor( (isqrt(5*(2n+1)^2)
 *            - (2n+1)) / 2 )
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001965_isqrt_seq, a001965_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001965_isqrt_seq.h"
#include "../utils/a001965_isqrt_u64.h"

seq_t a(idx_t n) {
  if (n < 900000000ULL) {
    uint64_t m = 2 * (uint64_t)n + 1;
    uint64_t s = a001965_isqrt_u64(5 * m * m);
    return (seq_t)((s - m) / 2);
  } else {
    seq_t m = (seq_t)n * 2 + 1;
    seq_t s = a001965_isqrt_seq(m * m * 5);
    return (s - m) / 2;
  }
}
