/*
 * A007746  Number of ways for n-3 nonintersecting loops to cross a line 2n
 *          times.
 * https://oeis.org/A007746
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (2*n)! * (n^4+20*n^3+107*n^2-107*n+15) / ( 3*(n-4)! *
 *            (n+6)! ) computed as 4 * P(n) * C(2n, n-4) / (3*(n+5)*(n+6)),
 *            with P(n) = n^4+20*n^3+107*n^2-107*n+15
 * Range:     u8 none, u16 n <= 7, u32 n <= 13, u64 n <= 28, u128 n <= 50,
 *            big any n
 * Helpers:   cancel_a7746, gcd_a7746
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cancel_a7746.h"
#include "../utils/gcd_a7746.h"

seq_t a(idx_t n) {
  seq_t m = (seq_t)n;
  seq_t p = m * (m * (m * (m + 20) + 107) - 107) + 15;

  /* C(2n, n-4) computed multiplicatively: r = C(n+4+i, i) after step i */
  seq_t c = 1;
  idx_t k = n - 4;
  for (idx_t i = 1; i <= k; i++) {
    c = c * ((seq_t)(n + 4) + (seq_t)i) / (seq_t)i;
  }

  seq_t d = (seq_t)3 * (m + 5) * (m + 6);
  seq_t four = 4;
  cancel_a7746(&p, &d);
  cancel_a7746(&four, &d);
  cancel_a7746(&c, &d);

  return four * p * c;
}
