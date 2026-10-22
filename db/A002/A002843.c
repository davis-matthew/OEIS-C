/*
 * A002843  Number of partitions of n into parts 1/2, 3/4, 7/8, 15/16, etc.
 * https://oeis.org/A002843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of compositions of n with each part <= 2*(previous
 *            part). Let g(r,m) = Sum_{k=1..m} f(r-k,k), where f(t,k) = number
 *            of ways to finish a remaining sum t when the last part was k.
 *            Then f(t,k) = g(t,min(2k,t)), f(0,k) = 1, and a(n) = g(n,n),
 *            a(0) = 1. Only g(r,2l) with 2l < r is kept;...
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"

static seq_t **rows = NULL; /* rows[r][l-1] = g(r, 2l), for 1 <= l <= (r-1)/2 */
static seq_t *A = NULL;     /* A[r] = a(r) */
static idx_t done = 0;      /* A[0..done-1] are computed */
static idx_t cap = 0;

seq_t a(idx_t n) {
  if (n >= done) {
    if (n + 1 > cap) {
      idx_t newcap = cap * 2;
      idx_t i;
      seq_t *nA;
      if (newcap < n + 1) newcap = n + 1;
      if (newcap < 16) newcap = 16;
      nA = zalloc_arr(newcap);
      for (i = 0; i < done; i++) nA[i] = A[i];
      A = nA;
      rows = (seq_t **)realloc(rows, (size_t)newcap * sizeof(seq_t *));
      for (i = cap; i < newcap; i++) rows[i] = NULL;
      cap = newcap;
    }
    {
      idx_t r;
      for (r = done; r <= n; r++) {
        idx_t L, m;
        seq_t *row;
        seq_t s;
        if (r == 0) {
          A[0] = 1;
          rows[0] = NULL;
          continue;
        }
        L = (r - 1) / 2;
        row = L ? zalloc_arr(L) : NULL;
        s = 0;
        for (m = 1; m <= r; m++) {
          idx_t t = r - m;
          if (2 * m >= t)
            s += A[t];
          else
            s += rows[t][m - 1];
          if (m % 2 == 0 && m < r) row[m / 2 - 1] = s;
        }
        A[r] = s;
        rows[r] = row;
      }
    }
    done = n + 1;
  }
  return A[n];
}
