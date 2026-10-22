/*
 * A002436  E.g.f.: Sum_{n >= 0} a(n)*x^(2*n)/(2*n)! = sec(2*x).
 * https://oeis.org/A002436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4^n * A000364(n), where A000364(n) = E(2n,2n) in the
 *            Entringer (Seidel) triangle: E(0,0) = 1, E(m,0) = 0 for m >= 1,
 *            E(m,k) = E(m,k-1) + E(m-1,m-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A002436_MAXN 12000
#define A002436_ROWLEN (2 * A002436_MAXN + 2)

static seq_t a002436_rowA[A002436_ROWLEN];
static seq_t a002436_rowB[A002436_ROWLEN];
static seq_t a002436_res[A002436_MAXN + 1];
static seq_t a002436_p4;
static idx_t a002436_row = 0;   /* index of the Entringer row held in prev */
static idx_t a002436_done = 0;  /* res[0..done-1] are valid */
static int a002436_init = 0;
static int a002436_which = 0;   /* 0: prev = rowA, 1: prev = rowB */

seq_t a(idx_t n) {
  if (n > A002436_MAXN) return 0;
  if (!a002436_init) {
    a002436_rowA[0] = 1;
    a002436_row = 0;
    a002436_which = 0;
    a002436_p4 = 1;
    a002436_res[0] = 1;
    a002436_done = 1;
    a002436_init = 1;
  }
  while (a002436_done <= n) {
    idx_t target = 2 * a002436_done;
    while (a002436_row < target) {
      seq_t *prev = a002436_which ? a002436_rowB : a002436_rowA;
      seq_t *cur = a002436_which ? a002436_rowA : a002436_rowB;
      idx_t m = a002436_row + 1;
      cur[0] = 0;
      for (idx_t k = 1; k <= m; k++) {
        cur[k] = cur[k - 1] + prev[m - k];
      }
      a002436_row = m;
      a002436_which ^= 1;
    }
    seq_t *prev = a002436_which ? a002436_rowB : a002436_rowA;
    a002436_p4 *= 4;
    a002436_res[a002436_done] = prev[target] * a002436_p4;
    a002436_done++;
  }
  return a002436_res[n];
}
