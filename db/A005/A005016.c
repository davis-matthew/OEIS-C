/*
 * A005016  Certain subgraphs of a directed graph.
 * https://oeis.org/A005016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum(x^n*Product((2^i-1)/(1+(2^i-2)*x),i = 1 .. n),n = 0 ..
 *            infinity), i.e. a(k) = Sum_{n=0..k} (-1)^(k-n) *
 *            Product_{i=1..n}(2^i-1) * h_{k-n}(c_1,...,c_n), where c_i =
 *            2^i-2 and h_m is the complete homogeneous symmetric polynomial,
 *            h_m(c_1..c_n) = h_m(c_1..c_{n-1}) + c_n*h_{m-1}(c_1..c_n)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t k) {
  seq_t pos = 0, neg = 0;
  seq_t *h = alloc_zero(k + 1);
  seq_t C = 1;   /* Product_{i=1..n} (2^i - 1) */
  seq_t p2 = 1;  /* 2^n */
  idx_t n, m;

  h[0] = 1;
  for (n = 0; n <= k; n++) {
    if (n > 0) {
      seq_t c;
      p2 *= 2;
      C *= p2 - 1;
      c = p2 - 2;
      for (m = 1; m <= k - n; m++) {
        h[m] += c * h[m - 1];
      }
    }
    if (((k - n) & 1) == 0)
      pos += C * h[k - n];
    else
      neg += C * h[k - n];
  }

  release(h, k + 1);
  return pos - neg;
}
