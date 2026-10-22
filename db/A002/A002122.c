/*
 * A002122  a(n) = Sum_{t=0..n} g(t)*g(n-t) where g(t) = A002121(t).
 * https://oeis.org/A002122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{t=0..n} g(t)*g(n-t), where g = A002121 has G.f.
 *            1/(1+Sum_{k>0} (-x)^prime(k)), i.e. g(0) = 1, g(m) = -g(m-2) +
 *            Sum_{odd primes p <= m} g(m-p)
 * Range:     i8 n <= 20, i16 n <= 48, i32 n <= 111, i64 n <= 241,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define A002122_MAX 32768

static seq_t a002122_g[A002122_MAX];
static long long a002122_glen = 0;
static int a002122_oddp[A002122_MAX];
static int a002122_np = 0;
static bool a002122_sieved = false;

static void a002122_sieve(void) {
  static bool comp[A002122_MAX];
  long long i, j;
  for (i = 2; i < A002122_MAX; i++) {
    if (!comp[i]) {
      if (i > 2) a002122_oddp[a002122_np++] = (int)i;
      for (j = i * i; j < A002122_MAX; j += i) comp[j] = true;
    }
  }
  a002122_sieved = true;
}

static void a002122_extend(long long upto) {
  long long m;
  int k;
  if (!a002122_sieved) a002122_sieve();
  for (m = a002122_glen; m <= upto; m++) {
    seq_t s = (seq_t)0;
    if (m == 0) {
      s = (seq_t)1;
    } else {
      if (m >= 2) s -= a002122_g[m - 2];
      for (k = 0; k < a002122_np && a002122_oddp[k] <= m; k++)
        s += a002122_g[m - a002122_oddp[k]];
    }
    a002122_g[m] = s;
  }
  if (upto + 1 > a002122_glen) a002122_glen = upto + 1;
}

seq_t a(idx_t n) {
  long long N = (long long)n;
  long long t;
  seq_t r = (seq_t)0;
  if (N >= A002122_MAX) return (seq_t)0;
  if (N >= a002122_glen) a002122_extend(N);
  for (t = 0; 2 * t < N; t++)
    r += a002122_g[t] * a002122_g[N - t];
  r = r * 2;
  if (N % 2 == 0) r += a002122_g[N / 2] * a002122_g[N / 2];
  return r;
}
