/*
 * A001828  Related to graded partially ordered sets.
 * https://oeis.org/A001828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p+q+r+s+t=n} (n!/p!q!r!s!t!) 2^(pq+qr+rs+st)
 *            computed as a(n) = Sum_{r+L+R=n} C(n,r) C(n-r,L) h(L,r) h(R,r),
 *            where h(L,r) = Sum_{q=0..L} C(L,q) 2^(q(L-q+r))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 102 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t a001828_h(idx_t L, idx_t r) {
  seq_t sum = 0;
  seq_t c = 1;
  for (idx_t q = 0; q <= L; q++) {
    sum += c * pow2(q * (L - q + r));
    if (q < L) c = c * (seq_t)(L - q) / (seq_t)(q + 1);
  }
  return sum;
}

static int a001828_width(void) {
  static int w = -1;
  if (w < 0) {
    seq_t x = 1;
    int k = 0;
    while (x != 0 && k < 300) {
      x <<= 1;
      k++;
    }
    w = (k >= 300) ? 0 : k;
  }
  return w;
}

seq_t a(idx_t n) {
  seq_t total = 0;
  seq_t cr = 1;
  for (idx_t r = 0; r <= n; r++) {
    idx_t m = n - r;
    seq_t cl = 1;
    for (idx_t L = 0; L <= m; L++) {
      idx_t R = m - L;
      total += cr * cl * a001828_h(L, r) * a001828_h(R, r);
      if (L < m) cl = cl * (seq_t)(m - L) / (seq_t)(L + 1);
    }
    if (r < n) cr = cr * (seq_t)(n - r) / (seq_t)(r + 1);
  }
  return total;
}
