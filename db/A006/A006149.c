/*
 * A006149  Number of 3-tuples (p_1, p_2, p_3) of Dyck paths of semilength n,
 *          such that each p_i is never below p_{i-1}.
 * https://oeis.org/A006149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 720 (2*n)! (2*n+2)! (2*n+4)! / (n! (n+1)! (n+2)! (n+3)!
 *            (n+4)! (n+5)!), evaluated exactly via prime exponents
 *            (Legendre's formula)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   leg_vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/leg_vp.h"

seq_t a(idx_t n) {
  uint64_t N = 2 * (uint64_t)n + 5;
  char *comp = (char *)calloc((size_t)(N + 1), 1);
  seq_t r = 1;
  uint64_t chunk = 1;
  uint64_t p, j;
  for (p = 2; p <= N; p++) {
    long long e;
    if (comp[p]) continue;
    for (j = p * p; j <= N; j += p) comp[j] = 1;
    e = (long long)leg_vp(2 * n, p) + (long long)leg_vp(2 * n + 2, p) +
        (long long)leg_vp(2 * n + 4, p);
    e -= (long long)leg_vp(n, p) + (long long)leg_vp(n + 1, p) +
         (long long)leg_vp(n + 2, p) + (long long)leg_vp(n + 3, p) +
         (long long)leg_vp(n + 4, p) + (long long)leg_vp(n + 5, p);
    if (p == 2) e += 4;
    else if (p == 3) e += 2;
    else if (p == 5) e += 1;
    while (e > 0) {
      if (chunk > UINT64_MAX / p) {
        r *= (seq_t)chunk;
        chunk = 1;
      }
      chunk *= p;
      e--;
    }
  }
  r *= (seq_t)chunk;
  free(comp);
  return r;
}
