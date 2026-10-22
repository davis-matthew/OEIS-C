/*
 * A007966  First factor in happy factorization of n.
 * https://oeis.org/A007966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; if n=k^2 then a(n)=k; else expand sqrt(n) as a continued
 *            fraction (P_k, Q_k, convergents p_k) to the middle of its
 *            period. If the period is odd, a(n)=1. If the period is 2m, let g
 *            = gcd(p_{m-1}, n), where p_{m-1}^2 - n*q_{m-1}^2 = (-1)^m*Q_m.
 *            Then a(n)=g when m is odd and a(n)=n/g when m...
 * Range:     u8 n <= 262, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   a7966_gcd, a7966_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a7966_gcd.h"
#include "../utils/a7966_isqrt.h"

seq_t a(idx_t n) {
  uint64_t N = (uint64_t)n;
  if (N == 0) return 0;
  uint64_t r = a7966_isqrt(N);
  if (r * r == N) return (seq_t)r;

  uint64_t Pprev = 0, Qprev = 1, ak = r;
  uint64_t pm2 = 1 % N, pm1 = r % N; /* p_{-1}, p_0 mod N */
  uint64_t k = 0;
  for (;;) {
    k++;
    uint64_t P = ak * Qprev - Pprev;
    uint64_t Q = (N - P * P) / Qprev;
    if (Q == Qprev) return 1; /* odd period */
    if (k >= 2 && P == Pprev) {
      /* even period, m = k-1; center Q_m = Qprev; convergent p_{m-1} = pm2 */
      uint64_t m = k - 1;
      uint64_t g = a7966_gcd(pm2, N);
      if (g == 0) g = N;
      if (m % 2 == 1) return (seq_t)g;
      return (seq_t)(N / g);
    }
    ak = (r + P) / Q;
    uint64_t pk = (uint64_t)(((unsigned __int128)ak * pm1 + pm2) % N);
    pm2 = pm1;
    pm1 = pk;
    Pprev = P;
    Qprev = Q;
  }
}
