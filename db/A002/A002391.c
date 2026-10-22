/*
 * A002391  Decimal expansion of natural logarithm of 3.
 * https://oeis.org/A002391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   log(3) = 2*arctanh(1/2) = Sum_{k>=0} (1/4)^k/(2k+1); a(n) = n-th
 *            digit of log(3) = 1.0986..., evaluated in base-10^9 fixed-point
 *            arithmetic with guard limbs
 * Range:     u8 n <= 8637, u16 n <= 8637, u32 n <= 8637, u64 n <= 8637,
 *            u128 n <= 8637, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A2391_BASE 1000000000ULL

static unsigned char *a2391_dig = NULL;
static size_t a2391_ndig = 0;

/* out[lo..L-1] = x[lo..L-1] / d, out[0..lo-1] = 0 */
static void a2391_div(uint32_t *out, const uint32_t *x, size_t L, size_t lo, uint64_t d) {
  uint64_t r = 0;
  size_t i;
  for (i = 0; i < lo; i++) out[i] = 0;
  for (i = lo; i < L; i++) {
    uint64_t cur = r * A2391_BASE + x[i];
    out[i] = (uint32_t)(cur / d);
    r = cur % d;
  }
}

static void a2391_compute(size_t N) {
  size_t L = 1 + (N + 8) / 9 + 3;
  uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
  uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
  uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
  size_t lo = 0, i, j;
  uint64_t k;
  P[0] = 1;
  for (k = 0;; k++) {
    while (lo < L && P[lo] == 0) lo++;
    if (lo >= L) break;
    a2391_div(T, P, L, lo, 2 * k + 1);
    {
      uint64_t carry = 0;
      size_t idx = L;
      while (idx > 0) {
        idx--;
        if (idx < lo && carry == 0) break;
        uint64_t s = (uint64_t)S[idx] + (idx >= lo ? T[idx] : 0) + carry;
        if (s >= A2391_BASE) { S[idx] = (uint32_t)(s - A2391_BASE); carry = 1; }
        else { S[idx] = (uint32_t)s; carry = 0; }
      }
    }
    a2391_div(P, P, L, lo, 4);
  }
  free(a2391_dig);
  a2391_dig = (unsigned char *)malloc(N);
  a2391_dig[0] = (unsigned char)(S[0] % 10);
  for (j = 1; j < N; j++) {
    size_t f = j - 1;
    uint32_t limb = S[1 + f / 9];
    unsigned pos = (unsigned)(f % 9);
    uint32_t div = 1;
    for (i = 0; i < 8 - pos; i++) div *= 10;
    a2391_dig[j] = (unsigned char)((limb / div) % 10);
  }
  a2391_ndig = N;
  free(P);
  free(S);
  free(T);
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if ((size_t)n > a2391_ndig) {
    size_t want = a2391_ndig * 2;
    if (want < 1024) want = 1024;
    if (want < (size_t)n) want = (size_t)n + 64;
    a2391_compute(want);
  }
  return (seq_t)a2391_dig[n - 1];
}
