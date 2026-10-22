/*
 * A000715  Number of partitions of n, with three kinds of 1,2 and 3 and two
 *          kinds of 4,5,6,....
 * https://oeis.org/A000715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 1/((1-x)*(1-x^2)*(1-x^3)*Product_{k>=1}(1-x^k)^2). p(n) =
 *            Sum_{k>=1} (-1)^(k+1) [p(n-k(3k-1)/2) + p(n-k(3k+1)/2)], p(0) =
 *            1 (A000041); q(n) = p(n) + Sum_{k>=1} (-1)^(k+1) [q(n-k(3k-1)/2)
 *            + q(n-k(3k+1)/2)] (A000712 = p*p); a = q divided by (1-x),
 *            (1-x^2), (1-x^3) via prefix sums
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 52, u64 n <= 178, u128 n <= 652,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"

static seq_t *A715_P = 0, *A715_Q = 0, *A715_A = 0;
static idx_t A715_cap = 0;

static void a715_build(idx_t N) {
  idx_t m, k, d, i;
  A715_P = alloc_zero(N);
  A715_Q = alloc_zero(N);
  A715_A = alloc_zero(N);
  for (m = 0; m < N; m++) {
    seq_t pos = 0, neg = 0;
    if (m == 0) pos = 1;
    for (k = 1;; k++) {
      idx_t g1 = k * (3 * k - 1) / 2, g2;
      seq_t t;
      if (g1 > m) break;
      t = A715_P[m - g1];
      g2 = g1 + k;
      if (g2 <= m) t += A715_P[m - g2];
      if (k & 1) pos += t; else neg += t;
    }
    A715_P[m] = pos - neg;

    pos = A715_P[m];
    neg = 0;
    for (k = 1;; k++) {
      idx_t g1 = k * (3 * k - 1) / 2, g2;
      seq_t t;
      if (g1 > m) break;
      t = A715_Q[m - g1];
      g2 = g1 + k;
      if (g2 <= m) t += A715_Q[m - g2];
      if (k & 1) pos += t; else neg += t;
    }
    A715_Q[m] = pos - neg;
    A715_A[m] = A715_Q[m];
  }
  for (d = 1; d <= 3; d++)
    for (i = d; i < N; i++)
      A715_A[i] += A715_A[i - d];
  A715_cap = N;
}

seq_t a(idx_t n) {
  if (n >= A715_cap) {
    idx_t N = 2 * A715_cap;
    if (N < n + 1) N = n + 1;
    if (N < 64) N = 64;
    a715_build(N);
  }
  return A715_A[n];
}
