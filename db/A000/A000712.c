/*
 * A000712  a(n) = number of partitions of n into parts of 2 kinds.
 * https://oeis.org/A000712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} p(k)*p(n-k), where p(n) = A000041(n) is
 *            computed by Euler's pentagonal recurrence p(m) = Sum_{k>=1}
 *            (-1)^(k+1) (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0) = 1
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 67, u64 n <= 216, u128 n <= 748,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *P = 0;
static idx_t Plen = 0;

static void ensure_p(idx_t n) {
  idx_t need = n + 1;
  if (need <= Plen) return;
  idx_t newlen = need;
  if (newlen < 2 * Plen) newlen = 2 * Plen;
  seq_t *q = alloc_zero(newlen);
  for (idx_t i = 0; i < Plen; i++) q[i] = P[i];
  idx_t start = Plen;
  if (start == 0) { q[0] = 1; start = 1; }
  for (idx_t m = start; m < newlen; m++) {
    seq_t pos = 0, neg = 0;
    for (idx_t k = 1;; k++) {
      idx_t g1 = k * (3 * k - 1) / 2;
      if (g1 > m) break;
      idx_t g2 = g1 + k;
      if (k & 1) {
        pos += q[m - g1];
        if (g2 <= m) pos += q[m - g2];
      } else {
        neg += q[m - g1];
        if (g2 <= m) neg += q[m - g2];
      }
    }
    q[m] = pos - neg;
  }
  P = q; /* old buffer intentionally not freed (element types may be non-trivial) */
  Plen = newlen;
}

seq_t a(idx_t n) {
  ensure_p(n);
  seq_t result = 0;
  for (idx_t k = 0; k <= n; k++) {
    result += P[k] * P[n - k];
  }
  return result;
}
