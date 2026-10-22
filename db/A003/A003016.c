/*
 * A003016  Number of occurrences of n as an entry in rows <= n of Pascal's
 *          triangle (A007318).
 * https://oeis.org/A003016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = #{(m,k): 0<=k<=m<=n, binomial(m,k) = n}; search over k
 *            with binomial(2k,k) <= n, walking m upward per k (a(1) = 3 by
 *            definition)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
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
    /* binomial(2k+2,k+1) = binomial(2k,k)*(2k+1)*(2k+2)/(k+1)^2 */
    ck = ck * (2 * k + 1) * (2 * k + 2) / ((unsigned __int128)(k + 1) * (k + 1));
  }
  return (seq_t)count;
}
