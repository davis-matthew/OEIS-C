/*
 * A004836  Numbers that are the sum of at most 7 nonzero 4th powers.
 * https://oeis.org/A004836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: a(n) is the n-th smallest m >= 0 with min number of
 *            nonzero 4th powers summing to m <= 7 (DP over m, growing limit)
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static size_t nterms = 0;
static size_t lim = 0;

static void build(size_t L) {
  unsigned char *c = (unsigned char *)malloc(L + 1);
  size_t i, k;
  free(terms);
  terms = (uint64_t *)malloc(sizeof(uint64_t) * (L + 1));
  nterms = 0;
  c[0] = 0;
  for (i = 1; i <= L; i++) {
    unsigned char best = 8;
    for (k = 1; k * k * k * k <= i; k++) {
      unsigned char t = c[i - k * k * k * k];
      if (t + 1 < best) best = (unsigned char)(t + 1);
      if (best == 1) break;
    }
    c[i] = best;
  }
  for (i = 0; i <= L; i++)
    if (c[i] <= 7) terms[nterms++] = (uint64_t)i;
  free(c);
  lim = L;
}

seq_t a(idx_t n) {
  while (nterms < n) {
    build(lim ? lim * 2 : 4096);
  }
  return (seq_t)(idx_t)terms[n - 1];
}
