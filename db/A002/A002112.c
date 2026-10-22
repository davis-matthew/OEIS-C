/*
 * A002112  Glaisher's H numbers.
 * https://oeis.org/A002112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 3/2 * (2n)! * [x^(2n)] 1/(2*cos(x)-1), i.e. a(n) =
 *            2*Sum_{k=1..n-1} (-1)^(k+1)*binomial(2n,2k)*a(n-k) +
 *            3*(-1)^(n+1), a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"

static seq_t *A_memo = NULL;   /* A_memo[m] = a(m) for 1 <= m <= done */
static seq_t *P_row = NULL;    /* current Pascal row, entries 0..row */
static idx_t cap = 0;          /* A_memo holds cap entries, P_row holds 2*cap */
static idx_t done = 0;
static idx_t row = 0;

static void ensure_cap(idx_t n) {
  idx_t newcap;
  if (n + 1 <= cap) return;
  newcap = cap * 2;
  if (newcap < n + 1) newcap = n + 1;
  if (newcap < 16) newcap = 16;
  if (cap == 0) {
    A_memo = a001028_z(newcap);
    P_row = a001028_z(2 * newcap);
    P_row[0] = 1;
    row = 0;
  } else {
    A_memo = a001028_grow(A_memo, cap, newcap);
    P_row = a001028_grow(P_row, 2 * cap, 2 * newcap);
  }
  cap = newcap;
}

static void advance_row(void) {
  idx_t j;
  P_row[row + 1] = 0;
  for (j = row + 1; j >= 1; j--) P_row[j] += P_row[j - 1];
  row++;
}

seq_t a(idx_t n) {
  idx_t m, k;
  if (n == 0) return 0;
  if (n <= done) return A_memo[n];
  ensure_cap(n);
  for (m = done + 1; m <= n; m++) {
    seq_t pos = 0, neg = 0;
    while (row < 2 * m) advance_row();
    for (k = 1; k + 1 <= m; k++) {
      seq_t t = P_row[2 * k] * A_memo[m - k];
      if (k & 1) pos += t;
      else neg += t;
    }
    pos += pos;
    neg += neg;
    if (m & 1) pos += 3;
    else neg += 3;
    A_memo[m] = pos - neg;
    done = m;
  }
  return A_memo[n];
}
