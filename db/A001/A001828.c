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
 * Helpers:   a001828_h, a001828_p2, a001828_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001828_h.h"
#include "../utils/a001828_p2.h"
#include "../utils/a001828_width.h"

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
