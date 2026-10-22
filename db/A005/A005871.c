/*
 * A005871  Theta series of hexagonal close-packing with respect to edge
 *          within layer.
 * https://oeis.org/A005871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of HCP lattice points at squared distance n/12
 *            from the midpoint of an in-layer edge (nearest-neighbour
 *            distance 1). Counted by direct enumeration over layers k: even
 *            layers: n = 8k^2 + 3(u^2 + 3b^2) with u == b+1 (mod 2); odd
 *            layers: n = 8k^2 + 3v^2 + (3b+1)^2 with v == b (mod 2)
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   a005871_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005871_isqrt.h"

static uint64_t a005871_isqrt_fast(uint64_t x) {
  uint64_t lo = 0, hi = 1;
  while (hi * hi <= x) hi <<= 1;
  while (hi - lo > 1) {
    uint64_t mid = lo + (hi - lo) / 2;
    if (mid * mid <= x) lo = mid; else hi = mid;
  }
  (void)a005871_isqrt;
  return lo;
}

/* count signed u with u*u == s and u == par (mod 2) */
static long long a005871_count(long long s, long long par) {
  if (s < 0) return 0;
  uint64_t r = a005871_isqrt_fast((uint64_t)s);
  if ((long long)(r * r) != s) return 0;
  if (((long long)r - par) % 2 != 0) return 0;
  return r == 0 ? 1 : 2;
}

seq_t a(idx_t n) {
  long long N = (long long)n;
  long long total = 0;
  long long B = (long long)a005871_isqrt_fast((uint64_t)N) + 2;
  long long k;
  for (k = -B; k <= B; k++) {
    long long r = N - 8 * k * k;
    if (r < 0) continue;
    long long b;
    if (k % 2 == 0) {
      if (r % 3 != 0) continue;
      long long t = r / 3;
      for (b = -B; b <= B; b++) {
        long long s = t - 3 * b * b;
        if (s < 0) continue;
        long long par = ((b + 1) % 2 + 2) % 2;
        total += a005871_count(s, par);
      }
    } else {
      for (b = -B; b <= B; b++) {
        long long w = 3 * b + 1;
        long long m = r - w * w;
        if (m < 0 || m % 3 != 0) continue;
        long long par = ((b % 2) + 2) % 2;
        total += a005871_count(m / 3, par);
      }
    }
  }
  return (seq_t)total;
}
