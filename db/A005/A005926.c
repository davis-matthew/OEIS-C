/*
 * A005926  Theta series of diamond with respect to midpoint of edge.
 * https://oeis.org/A005926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * #{(u,v,w) in Z^3 : u,v,w == 3 (mod 4), u+v+w == 5
 *            (mod 8), u^2+v^2+w^2 = n} (diamond scaled by 4, centred at an
 *            edge midpoint; the factor 2 counts the two fcc cosets, which map
 *            onto each other under negation). Computed by enumerating u, v
 * Range:     u8 n <= 1600, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   a005926_isqrt, a005926_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005926_isqrt.h"
#include "../utils/a005926_mod.h"

static long long a005926_isqrt_fast(long long x) {
  if (x < 0) return -1;
  long long r = 0, b = 1;
  while (b * b <= x) b <<= 1;
  for (; b > 0; b >>= 1) {
    long long t = r + b;
    if (t * t <= x) r = t;
  }
  (void)a005926_isqrt;
  return r;
}

seq_t a(idx_t n) {
  long long N = (long long)n;
  long long r = a005926_isqrt_fast(N);
  long long count = 0;
  for (long long u = -r; u <= r; u++) {
    if (a005926_mod(u, 4) != 3) continue;
    long long ru = N - u * u;
    for (long long v = -r; v <= r; v++) {
      if (a005926_mod(v, 4) != 3) continue;
      long long rem = ru - v * v;
      if (rem < 0) continue;
      long long w = a005926_isqrt_fast(rem);
      if (w * w != rem) continue;
      if (a005926_mod(w, 4) == 3 && a005926_mod(u + v + w, 8) == 5) count++;
      if (w != 0 && a005926_mod(-w, 4) == 3 && a005926_mod(u + v - w, 8) == 5) count++;
    }
  }
  return (seq_t)2 * (seq_t)count;
}
