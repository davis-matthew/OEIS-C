/*
 * A002555  Denominators of coefficients for numerical differentiation.
 * https://oeis.org/A002555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((-1)^(n-1)*A001824(n-1)/(2^(2n-3)*(2n)!)).
 *            Since A001824(n-1) = ((2n-1)!!)^2 * H(n) with H(n) =
 *            Sum_{k=1..n} 1/(2k-1)^2, the value equals (2n-1)!! * H(n) / (n!
 *            * 2^(3n-3)). The denominator is Prod_p p^max(0, -v_p(value)),
 *            where v_2(value) = v_2(H) - v_2(n!) - (3n-3) and, for od...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   a2555_leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2555_leg.h"

seq_t a(idx_t n) {
  uint64_t L = 2 * (uint64_t)n - 1;
  seq_t r = (seq_t)1;

  /* p = 2: H = A/B with B odd, computed modulo 2^128 */
  {
    unsigned __int128 A = 0, B = 1;
    uint64_t k;
    int v2h = 0;
    long long e2;
    for (k = 1; k <= (uint64_t)n; k++) {
      unsigned __int128 x = (unsigned __int128)(2 * k - 1);
      unsigned __int128 x2 = x * x;
      A = A * x2 + B;
      B = B * x2;
    }
    if (A == 0) v2h = 128;
    else while (((A >> v2h) & 1) == 0) v2h++;
    e2 = (long long)a2555_leg((uint64_t)n, 2) + 3 * (long long)n - 3 - v2h;
    while (e2-- > 0) r *= 2;
  }

  if (L < 3) return r;

  {
    char *comp = (char *)calloc((size_t)L + 1, 1);
    uint64_t p, i;
    for (p = 2; p * p <= L; p++)
      if (!comp[p])
        for (i = p * p; i <= L; i += p) comp[i] = 1;

    for (p = 3; p <= L; p += 2) {
      uint64_t pm = 1, M, A = 0, x;
      int m = 0;
      long long vH, vb, e;
      if (comp[p]) continue;
      while (pm <= L / p) { pm *= p; m++; }
      M = pm * pm;
      /* S = p^(2m) * H mod M; terms with p not dividing x vanish mod M */
      for (x = p; x <= L; x += 2 * p) {
        uint64_t u = x, c = 1, inv, t;
        int v = 0, j;
        while (u % p == 0) { u /= p; v++; }
        for (j = 0; j < 2 * (m - v); j++) c = c * p % M;
        /* inverse of u^2 modulo M via extended Euclid */
        {
          uint64_t u2 = (u % M) * (u % M) % M;
          long long r0 = (long long)M, r1 = (long long)u2, s0 = 0, s1 = 1;
          while (r1 != 0) {
            long long q = r0 / r1, tmp;
            tmp = r0 - q * r1; r0 = r1; r1 = tmp;
            tmp = s0 - q * s1; s0 = s1; s1 = tmp;
          }
          s0 %= (long long)M;
          if (s0 < 0) s0 += (long long)M;
          inv = (uint64_t)s0;
        }
        t = c * inv % M;
        A = (A + t) % M;
      }
      if (A == 0) continue; /* v_p(H) >= 0, no contribution */
      {
        int va = 0;
        uint64_t y = A;
        while (y % p == 0) { y /= p; va++; }
        vH = (long long)va - 2 * (long long)m;
      }
      vb = (long long)a2555_leg(2 * (uint64_t)n, p) - 2 * (long long)a2555_leg((uint64_t)n, p);
      e = -(vH + vb);
      while (e-- > 0) r *= (seq_t)p;
    }
    free(comp);
  }
  return r;
}
