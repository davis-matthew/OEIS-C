/*
 * A005703  Number of n-node connected graphs with at most one cycle.
 * https://oeis.org/A005703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000055(n) + A001429(n), with R(x) = Sum r(k) x^k the
 *            g.f. of rooted trees A000081 (r(1) = 1, r(n+1) = (1/n)
 *            Sum_{k=1..n} (Sum_{d|k} d r(d)) r(n-k+1)). Trees (Otter):
 *            A000055(n) = r(n) - (Sum_{i+j=n} r(i)r(j) - [n even] r(n/2))/2,
 *            A000055(0) = 1. Unicyclic graphs (dihedral Burnside over cy...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"

#define FIX_LIM 120
#define NPR 6

static uint64_t pw_mod(uint64_t b, uint64_t e, uint64_t p) {
  uint64_t res = 1 % p;
  b %= p;
  while (e) {
    if (e & 1) res = res * b % p;
    b = b * b % p;
    e >>= 1;
  }
  return res;
}

static bool small_prime(uint64_t m) {
  uint64_t d;
  if (m < 2) return false;
  for (d = 2; d * d <= m; d++)
    if (m % d == 0) return false;
  return true;
}

static void mod_compute(idx_t L, uint64_t p, uint64_t *out) {
  idx_t n1 = L + 1, i, j, m, k;
  uint64_t *r = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *s = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *c = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *q = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *cf = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *rr = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *r2 = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *tt = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *num = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  uint64_t *y = (uint64_t *)calloc(n1 + 1, sizeof(uint64_t));
  idx_t *phi = (idx_t *)calloc(n1 + 1, sizeof(idx_t));
  uint64_t inv2 = pw_mod(2, p - 2, p), inv4 = pw_mod(4, p - 2, p);

  if (L >= 1) r[1] = 1;
  for (m = 1; m < L; m++) {
    uint64_t acc = 0;
    for (k = m; k <= L; k += m) s[k] = (s[k] + (m % p) * r[m]) % p;
    for (k = 1; k <= m; k++) acc = (acc + s[k] * r[m - k + 1]) % p;
    r[m + 1] = acc * pw_mod(m % p, p - 2, p) % p;
  }
  c[0] = 1;
  for (j = 1; j <= L; j++) {
    uint64_t acc = 0, acq = 0;
    for (i = 1; i <= j; i++) {
      acc = (acc + r[i] * c[j - i]) % p;
      acq = (acq + (i % p) * r[i] % p * c[j - i]) % p;
    }
    c[j] = acc;
    q[j] = acq;
  }
  for (i = 0; i <= L; i++) phi[i] = i;
  for (i = 2; i <= L; i++)
    if (phi[i] == i)
      for (j = i; j <= L; j += i) phi[j] -= phi[j] / i;
  for (i = 1; i <= L; i++)
    for (j = i; j <= L; j += i)
      cf[j] = (cf[j] + (phi[i] % p) * q[j / i]) % p;
  for (j = 1; j <= L; j++) cf[j] = cf[j] * pw_mod(j % p, p - 2, p) % p;
  for (j = 0; j <= L; j++) {
    uint64_t acc = 0;
    for (i = 1; i < j; i++) acc = (acc + r[i] * r[j - i]) % p;
    rr[j] = acc;
    r2[j] = (j % 2 == 0) ? r[j / 2] : 0;
    tt[j] = (2 * r[j] + r2[j] + rr[j]) % p;
  }
  for (j = 0; j <= L; j++) {
    uint64_t acc = 0;
    for (i = 2; i <= j; i += 2) acc = (acc + r2[i] * tt[j - i]) % p;
    num[j] = acc;
  }
  for (j = 0; j <= L; j++) {
    uint64_t acc = 0;
    for (i = 0; i <= j; i += 2) acc = (acc + c[i / 2] * num[j - i]) % p;
    y[j] = acc;
  }
  out[0] = 1 % p;
  for (j = 1; j <= L; j++) {
    uint64_t t = (r[j] + p - (rr[j] + p - r2[j]) % p * inv2 % p) % p;
    uint64_t u = (2 * cf[j] + y[j] + p - tt[j]) % p * inv4 % p;
    out[j] = (t + u) % p;
  }
  free(r); free(s); free(c); free(q); free(cf); free(rr);
  free(r2); free(tt); free(num); free(y); free(phi);
}

static unsigned __int128 fix_tab[FIX_LIM + 1];
static bool fix_done = false;

static void fix_compute(void) {
  uint64_t pr[NPR];
  uint64_t *res[NPR];
  uint64_t cand = 2147483647ULL;
  int i, j, cnt = 0;
  idx_t n;
  while (cnt < NPR) {
    if (small_prime(cand)) pr[cnt++] = cand;
    cand--;
  }
  for (i = 0; i < NPR; i++) {
    res[i] = (uint64_t *)calloc(FIX_LIM + 1, sizeof(uint64_t));
    mod_compute(FIX_LIM, pr[i], res[i]);
  }
  for (n = 0; n <= FIX_LIM; n++) {
    uint64_t cc[NPR];
    unsigned __int128 v;
    for (i = 0; i < NPR; i++) {
      uint64_t x = res[i][n];
      for (j = 0; j < i; j++) {
        x = (x + pr[i] - cc[j] % pr[i]) % pr[i];
        x = x * pw_mod(pr[j] % pr[i], pr[i] - 2, pr[i]) % pr[i];
      }
      cc[i] = x;
    }
    v = cc[NPR - 1];
    for (i = NPR - 2; i >= 0; i--) v = v * pr[i] + cc[i];
    fix_tab[n] = v;
  }
  for (i = 0; i < NPR; i++) free(res[i]);
  fix_done = true;
}

static seq_t *big_a = NULL;
static idx_t big_len = 0;

static void big_compute(idx_t L) {
  idx_t n1 = L + 2, i, j, m, k;
  seq_t *r = alloc_zero(n1), *s = alloc_zero(n1), *c = alloc_zero(n1);
  seq_t *q = alloc_zero(n1), *cf = alloc_zero(n1), *rr = alloc_zero(n1);
  seq_t *r2 = alloc_zero(n1), *tt = alloc_zero(n1), *num = alloc_zero(n1);
  seq_t *y = alloc_zero(n1), *out = alloc_zero(n1);
  idx_t *phi = (idx_t *)calloc(n1, sizeof(idx_t));

  if (L >= 1) r[1] = 1;
  for (m = 1; m < L; m++) {
    seq_t acc = 0;
    for (k = m; k <= L; k += m) s[k] += (seq_t)m * r[m];
    for (k = 1; k <= m; k++) acc += s[k] * r[m - k + 1];
    r[m + 1] = acc / (seq_t)m;
  }
  c[0] = 1;
  for (j = 1; j <= L; j++) {
    seq_t acc = 0, acq = 0;
    for (i = 1; i <= j; i++) {
      acc += r[i] * c[j - i];
      acq += (seq_t)i * r[i] * c[j - i];
    }
    c[j] = acc;
    q[j] = acq;
  }
  for (i = 0; i <= L; i++) phi[i] = i;
  for (i = 2; i <= L; i++)
    if (phi[i] == i)
      for (j = i; j <= L; j += i) phi[j] -= phi[j] / i;
  for (i = 1; i <= L; i++)
    for (j = i; j <= L; j += i)
      cf[j] += (seq_t)phi[i] * q[j / i];
  for (j = 1; j <= L; j++) cf[j] = cf[j] / (seq_t)j;
  for (j = 0; j <= L; j++) {
    seq_t acc = 0;
    for (i = 1; i < j; i++) acc += r[i] * r[j - i];
    rr[j] = acc;
    if (j % 2 == 0) r2[j] = r[j / 2];
    tt[j] = r[j] * 2 + r2[j] + rr[j];
  }
  for (j = 0; j <= L; j++) {
    seq_t acc = 0;
    for (i = 2; i <= j; i += 2) acc += r2[i] * tt[j - i];
    num[j] = acc;
  }
  for (j = 0; j <= L; j++) {
    seq_t acc = 0;
    for (i = 0; i <= j; i += 2) acc += c[i / 2] * num[j - i];
    y[j] = acc;
  }
  out[0] = 1;
  for (j = 1; j <= L; j++) {
    seq_t t = r[j] - (rr[j] - r2[j]) / 2;
    seq_t u = (cf[j] * 2 + y[j] - tt[j]) / 4;
    out[j] = t + u;
  }
  release(r, n1); release(s, n1); release(c, n1); release(q, n1);
  release(cf, n1); release(rr, n1); release(r2, n1); release(tt, n1);
  release(num, n1); release(y, n1);
  free(phi);
  if (big_a) release(big_a, big_len);
  big_a = out;
  big_len = n1;
}

seq_t a(idx_t n) {
  if (!seq_is_big()) {
    if (n > FIX_LIM) return 0;
    if (!fix_done) fix_compute();
    return u128_to_seq(fix_tab[n]);
  }
  if (n + 1 >= big_len) {
    idx_t L = n + 16;
    if (L < big_len + big_len / 2) L = big_len + big_len / 2;
    big_compute(L);
  }
  return big_a[n];
}
