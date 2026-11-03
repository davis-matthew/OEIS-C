/*
 * A002142  Primes p == 1 (mod 4) where class number of Q(sqrt p) increases.
 * https://oeis.org/A002142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p == 1 (mod 4), keeping those where h(p) sets
 *            a new record. R = log(eps) comes from the continued-fraction
 *            period of (1+sqrt(p))/2. We skip p when R >
 *            sqrt(p)*(log(p)/2+1/2)/(2*(record+1)), because then h(p) cannot
 *            exceed the record (Louboutin's bound on L(1,chi)). Otherwise
 *            h...
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64, jacobi, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"
#include "../utils/jacobi.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static double a2142_e1(double x) {
  if (x <= 1.0) {
    double sum = 0.0, term = 1.0;
    int k;
    for (k = 1; k < 200; k++) {
      term *= -x / k;
      double t = term / k;
      sum += t;
      if (fabs(t) < 1e-18) break;
    }
    return -0.57721566490153286061 - log(x) - sum;
  } else {
    const double tiny = 1e-300;
    double b = x + 1.0, c = 1.0 / tiny, d = 1.0 / b, h = d;
    int i;
    for (i = 1; i < 1000; i++) {
      double an = -(double)i * i;
      b += 2.0;
      d = 1.0 / (an * d + b);
      c = b + an / c;
      double del = c * d;
      h *= del;
      if (fabs(del - 1.0) < 1e-16) break;
    }
    return h * exp(-x);
  }
}

/* returns regulator, or -1 if it exceeds T */
static double a2142_reg(uint64_t p, double T) {
  long long D = (long long)p, s = (long long)isqrt_u64(p);
  double sd = sqrt((double)p);
  long long P = 1, Q = 2, a0;
  a0 = (P + s) / Q;
  P = a0 * Q - P;
  Q = (D - P * P) / Q;
  long long P1 = P, Q1 = Q;
  double R = 0.0, prod = 1.0;
  double lim = T > 230.0 ? 1e100 : exp(T);
  do {
    double xi = ((double)P + sd) / (double)Q;
    prod *= xi;
    if (prod > lim) {
      R += log(prod);
      prod = 1.0;
      if (R > T) return -1.0;
      double r = T - R;
      lim = r > 230.0 ? 1e100 : exp(r);
    }
    long long aa = (P + s) / Q;
    long long Pn = aa * Q - P;
    long long Qn = (D - Pn * Pn) / Q;
    P = Pn; Q = Qn;
  } while (P != P1 || Q != Q1);
  R += log(prod);
  return R;
}

static const double A2142_PI = 3.14159265358979323846;

static long long a2142_classno(uint64_t p, double R) {
  double sp = sqrt((double)p), c = sqrt(A2142_PI / (double)p);
  double S = 0.0;
  uint64_t n;
  for (n = 1;; n++) {
    double x = A2142_PI * (double)n * (double)n / (double)p;
    if (x > 50.0) break;
    int chi = jacobi(n % p, p);
    if (chi == 0) continue;
    double t = sp / (double)n * erfc((double)n * c) + a2142_e1(x);
    S += chi * t;
  }
  return (long long)floor(S / (2.0 * R) + 0.5);
}

static uint64_t *a2142_rec = NULL;
static idx_t a2142_cnt = 0, a2142_cap = 0;
static uint64_t a2142_next = 5;
static long long a2142_best = 0;

seq_t a(idx_t n) {
  if (n == 0) return 0;
  while (a2142_cnt < n) {
    uint64_t p = a2142_next;
    a2142_next += 4;
    if (!is_prime_u64(p)) continue;
    double lp = log((double)p);
    double T = sqrt((double)p) * (0.5 * lp + 0.5) / (2.0 * (double)(a2142_best + 1)) + 1e-9;
    double R = a2142_reg(p, T);
    if (R < 0) continue;
    long long h = a2142_classno(p, R);
    if (h > a2142_best) {
      a2142_best = h;
      if (a2142_cnt == a2142_cap) {
        idx_t nc = a2142_cap ? a2142_cap * 2 : 64;
        a2142_rec = (uint64_t *)realloc(a2142_rec, (size_t)nc * sizeof(uint64_t));
        a2142_cap = nc;
      }
      a2142_rec[a2142_cnt++] = p;
    }
  }
  return (seq_t)a2142_rec[n - 1];
}
