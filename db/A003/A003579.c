/*
 * A003579  Dowling numbers: e.g.f. exp(x + (exp(b*x) - 1)/b), with b=7.
 * https://oeis.org/A003579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = exp(x + (exp(7x)-1)/7) satisfies A^(n)(x) =
 *            A(x)*P_n(exp(7x)), P_0 = 1, P_{n+1}(y) = (1+y)*P_n(y) +
 *            7*y*P_n'(y); so with c(0,0) = 1, c(n+1,j) = (7*j+1)*c(n,j) +
 *            c(n,j-1), and a(n) = Sum_{j=0..n} c(n,j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *row = NULL;   /* row[j] = c(m, j) for the current row m */
static seq_t *vals = NULL;  /* vals[i] = a(i) for i <= m */
static idx_t cap = 0;
static idx_t m = 0;

static void ensure_cap(idx_t need) {
  if (need <= cap) return;
  idx_t ncap = cap ? cap : 64;
  while (ncap < need) ncap *= 2;
  row = A_grow(row, cap, ncap);
  vals = A_grow(vals, cap, ncap);
  for (idx_t i = cap; i < ncap; i++) {
    row[i] = 0;
    vals[i] = 0;
  }
  cap = ncap;
}

seq_t a(idx_t n) {
  if (cap == 0) {
    ensure_cap(64);
    row[0] = 1;
    vals[0] = 1;
    m = 0;
  }
  if (n <= m) return vals[n];
  ensure_cap(n + 2);
  while (m < n) {
    idx_t nm = m + 1;
    row[nm] = 0;
    for (idx_t j = nm; j >= 1; j--) {
      row[j] = row[j] * (seq_t)(7 * j + 1) + row[j - 1];
    }
    seq_t s = 0;
    for (idx_t j = 0; j <= nm; j++) s += row[j];
    vals[nm] = s;
    m = nm;
  }
  return vals[n];
}
