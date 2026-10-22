/*
 * A007182  Dimension of n-th compound of a certain space.
 * https://oeis.org/A007182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = Product_{k>=1} 1/(1-x^k)^c(k), where c(1) = 2 and
 *            c(k) = a(floor(k/2)) for k >= 2; a(0) = 1. (The Euler transform
 *            of 2,2,2,5,5,10,10,23,23,45,45,...)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 83, u128 n <= 188,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *tab = NULL;
static idx_t tab_len = 0;

static uint64_t small_gcd(uint64_t x, uint64_t y) {
  while (y) {
    uint64_t t = x % y;
    x = y;
    y = t;
  }
  return x;
}

static void build(idx_t N) {
  seq_t *f = alloc_zero(N + 1);
  seq_t *C = alloc_zero(N + 1);
  idx_t k, j, n;
  f[0] = 1;
  for (k = 1; k <= N; k++) {
    seq_t b;
    idx_t J = N / k;
    if (k == 1)
      b = 2;
    else
      b = f[k / 2];
    /* C[j] = binomial(b + j - 1, j), computed exactly without overflow */
    C[0] = 1;
    for (j = 1; j <= J; j++) {
      uint64_t r = (uint64_t)(long long)(C[j - 1] % (seq_t)j);
      uint64_t g = small_gcd((uint64_t)j, r);
      uint64_t d = (uint64_t)j / g;
      seq_t t = (b + (seq_t)(j - 1)) / (seq_t)d;
      C[j] = (C[j - 1] / (seq_t)g) * t;
    }
    /* f <- f * (1 - x^k)^(-b), in place, from the top down */
    for (n = N; n >= k; n--) {
      seq_t s = f[n];
      for (j = 1; j * k <= n; j++)
        s += C[j] * f[n - j * k];
      f[n] = s;
    }
  }
  release(C, N + 1);
  if (tab) release(tab, tab_len);
  tab = f;
  tab_len = N + 1;
}

seq_t a(idx_t n) {
  if (n >= tab_len) {
    idx_t N = n;
    if (N < 64) N = 64;
    if (tab_len > 0 && N < 2 * tab_len) N = 2 * tab_len;
    build(N);
  }
  return tab[n];
}
