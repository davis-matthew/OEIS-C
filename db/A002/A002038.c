/*
 * A002038  Numbers dividing A002037(i) and larger than A002037(i-1), for some
 *          i>0.
 * https://oeis.org/A002038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; then, for i = 1, 2, ..., list in increasing order the
 *            divisors d of A002037(i) with d > A002037(i-1). Here A002037(0)
 *            = 1, and A002037(i) is the product of the first k_i primes,
 *            where k_i = bitlength(A002037(i-1)) + 1. This gives 1, 6 = 3#,
 *            210 = 7#, 223092870 = 23#, 109#, ... Blocks 1-...
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   c2038_bitlen, c2038_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c2038_bitlen.h"
#include "../utils/c2038_cmp.h"

static uint64_t *c2038 = 0;
static size_t c2038_len = 0, c2038_cap = 0;
static uint64_t c2038_next_lo = 0;
static int c2038_k = 0;
static int c2038_primes[64];

static void c2038_push(uint64_t v) {
  if (c2038_len == c2038_cap) {
    size_t nc = c2038_cap ? 2 * c2038_cap : 1024;
    c2038 = (uint64_t *)realloc(c2038, nc * sizeof(uint64_t));
    c2038_cap = nc;
  }
  c2038[c2038_len++] = v;
}

static void c2038_init(void) {
  int cnt = 0;
  int m;
  int i, k, j;
  uint64_t P, Q;
  for (m = 2; cnt < 64; m++) {
    int ok = 1, d;
    for (d = 2; d * d <= m; d++) if (m % d == 0) { ok = 0; break; }
    if (ok) c2038_primes[cnt++] = m;
  }
  c2038_push(1);
  P = 1;
  for (i = 1; i <= 3; i++) {
    uint64_t *divs;
    size_t nd = 1, sel = 0, t;
    k = c2038_bitlen(P) + 1;
    Q = 1;
    for (j = 0; j < k; j++) Q *= (uint64_t)c2038_primes[j];
    divs = (uint64_t *)malloc(((size_t)1 << k) * sizeof(uint64_t));
    divs[0] = 1;
    for (j = 0; j < k; j++) {
      size_t cur = nd;
      for (t = 0; t < cur; t++) divs[nd++] = divs[t] * (uint64_t)c2038_primes[j];
    }
    for (t = 0; t < nd; t++) if (divs[t] > P) divs[sel++] = divs[t];
    qsort(divs, sel, sizeof(uint64_t), c2038_cmp);
    for (t = 0; t < sel; t++) c2038_push(divs[t]);
    free(divs);
    P = Q;
  }
  c2038_k = c2038_bitlen(P) + 1;
  c2038_next_lo = P + 1;
}

#define C2038_SEG 65536
static uint64_t c2038_rem[C2038_SEG];
static unsigned char c2038_bad[C2038_SEG];

static void c2038_segment(void) {
  uint64_t lo = c2038_next_lo, hi = lo + C2038_SEG;
  size_t j;
  int t;
  for (j = 0; j < C2038_SEG; j++) { c2038_rem[j] = lo + j; c2038_bad[j] = 0; }
  for (t = 0; t < c2038_k; t++) {
    uint64_t p = (uint64_t)c2038_primes[t];
    uint64_t x = (lo + p - 1) / p * p;
    for (; x < hi; x += p) {
      size_t q = (size_t)(x - lo);
      c2038_rem[q] /= p;
      if (c2038_rem[q] % p == 0) c2038_bad[q] = 1;
    }
  }
  for (j = 0; j < C2038_SEG; j++)
    if (!c2038_bad[j] && c2038_rem[j] == 1) c2038_push(lo + j);
  c2038_next_lo = hi;
}

seq_t a(idx_t n) {
  if (c2038_len == 0) c2038_init();
  while ((idx_t)c2038_len < n) c2038_segment();
  return (seq_t)c2038[n - 1];
}
