/*
 * A002512  Expansion of chi(x)^10 / phi(x)^4 in powers of x where phi(),
 *          chi() are Ramanujan theta functions.
 * https://oeis.org/A002512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>0} ((1 - x^k) * (1 - x^(4*k)))^-2, a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 59, u64 n <= 186, u128 n <= 630,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *a002512_tab = NULL;
static idx_t a002512_cap = 0;

static void a002512_build(idx_t N) {
  seq_t *c = alloc_zero(N + 1);
  c[0] = 1;
  for (idx_t k = 1; k <= N; k++) {
    for (int pass = 0; pass < 2; pass++)
      for (idx_t i = k; i <= N; i++)
        c[i] += c[i - k];
    idx_t k4 = 4 * k;
    if (k4 <= N) {
      for (int pass = 0; pass < 2; pass++)
        for (idx_t i = k4; i <= N; i++)
          c[i] += c[i - k4];
    }
  }
  a002512_tab = c;
  a002512_cap = N + 1;
}

seq_t a(idx_t n) {
  if (n >= a002512_cap) {
    idx_t N = n < 64 ? 64 : n;
    if (a002512_cap > 0 && N < 2 * a002512_cap) N = 2 * a002512_cap;
    a002512_build(N);
  }
  return a002512_tab[n];
}
