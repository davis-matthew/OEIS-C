/*
 * A005157  Number of totally symmetric plane partitions that fit in an n X n
 *          X n box.
 * https://oeis.org/A005157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product_{i=1..n} Product_{j=i..n} Product_{k=j..n}
 *            (i+j+k-1)/(i+j+k-2); grouped by s = i+j+k, so each integer m
 *            gets exponent c(m+1) - c(m+2), where c(s) counts the triples 1
 *            <= i <= j <= k <= n with i+j+k = s; the result is built from
 *            prime exponents
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 24,
 *            big any n
 * Helpers:   a005157_powmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a005157_powmul.h"

seq_t a(idx_t n) {
  if (n == 0) return (seq_t)1;
  long long N = (long long)n;
  long long M = 3 * N + 3;
  long long *c = (long long *)calloc((size_t)(M + 2), sizeof(long long));
  long long *ex = (long long *)calloc((size_t)(M + 2), sizeof(long long));
  int *spf = (int *)calloc((size_t)(M + 2), sizeof(int));
  long long i, j, m, s;

  /* difference array: for fixed i<=j, k runs j..n, so s runs i+2j .. i+j+n */
  for (i = 1; i <= N; i++) {
    for (j = i; j <= N; j++) {
      c[i + 2 * j] += 1;
      c[i + j + N + 1] -= 1;
    }
  }
  for (s = 1; s <= M + 1; s++) c[s] += c[s - 1];

  /* smallest prime factor sieve */
  for (m = 2; m <= M; m++) {
    if (spf[m] == 0) {
      for (j = m; j <= M; j += m)
        if (spf[j] == 0) spf[j] = (int)m;
    }
  }

  /* exponent of integer m is c(m+1) - c(m+2); distribute to primes */
  for (m = 2; m + 2 <= M + 1; m++) {
    long long e = c[m + 1] - c[m + 2];
    if (e == 0) continue;
    long long x = m;
    while (x > 1) {
      long long p = spf[x];
      while (x % p == 0) {
        x /= p;
        ex[p] += e;
      }
    }
  }

  seq_t result = 1;
  for (m = 2; m <= M; m++) {
    if (ex[m] > 0) result *= a005157_powmul((seq_t)m, ex[m]);
  }

  free(c);
  free(ex);
  free(spf);
  return result;
}
