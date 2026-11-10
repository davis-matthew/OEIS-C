/*
 * A085247  Duplicate of A003016.
 * https://oeis.org/A085247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A003016(n); a(n) = #{(m,k): 0<=k<=m<=n, binomial(m,k) =
 *            n}, by search over k (a(0)=0, a(1)=3)
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A003016_a(idx_t n) {
  uint64_t N = (uint64_t)n;
  uint64_t count = 0;
  uint64_t k;
  unsigned __int128 ck;

  if (N == 0) return (seq_t)0;
  if (N == 1) return (seq_t)3;

  ck = 2; /* binomial(2,1) */
  for (k = 1; ck <= (unsigned __int128)N; k++) {
    unsigned __int128 c = ck;
    uint64_t m = 2 * k;
    while (c < (unsigned __int128)N) {
      c = c * (m + 1) / (m + 1 - k);
      m++;
    }
    if (c == (unsigned __int128)N)
      count += (m == 2 * k) ? 1 : 2;
    ck = ck * (2 * k + 1) * (2 * k + 2) / ((unsigned __int128)(k + 1) * (k + 1));
  }
  return (seq_t)count;
}

seq_t a(idx_t n) {
  return A003016_a(n);
}
