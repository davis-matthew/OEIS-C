/*
 * A006209  Generalized Fibonacci numbers A_{n,4}.
 * https://oeis.org/A006209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(n/d)*L(d), where L(d) = p(d) - (-1)^d
 *            is the d-th power sum of the roots of
 *            x^8-x^7-x^6+x^5-x^4+x^3-x^2+x-1 = (x^9-2*x^7-1)/(x+1), and p(n)
 *            = 2*p(n-2) + p(n-9) for n > 9, p(1..9) = 0,4,0,8,0,16,0,32,9
 *            (power sums of the roots of x^9-2*x^7-1)
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   mobius_u64, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/mobius_u64.h"

/* 192-bit unsigned helpers (three 64-bit limbs, little-endian) */
static void w_add(uint64_t *x, const uint64_t *y) {
  uint64_t c = 0;
  int i;
  for (i = 0; i < 3; i++) {
    uint64_t s = x[i] + y[i];
    uint64_t c1 = (s < x[i]);
    uint64_t t = s + c;
    uint64_t c2 = (t < s);
    x[i] = t;
    c = c1 | c2;
  }
}

static void w_sub(uint64_t *x, const uint64_t *y) {
  uint64_t b = 0;
  int i;
  for (i = 0; i < 3; i++) {
    uint64_t d = x[i] - y[i];
    uint64_t b1 = (x[i] < y[i]);
    uint64_t t = d - b;
    uint64_t b2 = (d < b);
    x[i] = t;
    b = b1 | b2;
  }
}

static void w_set(uint64_t *x, uint64_t v) {
  x[0] = v; x[1] = 0; x[2] = 0;
}

static seq_t a_fixed(idx_t n) {
  uint64_t w[9][3];
  uint64_t pos[3], neg[3], cur[3], one[3], L[3];
  idx_t k;
  int i;
  for (i = 0; i < 9; i++) w_set(w[i], 0);
  w_set(pos, 0); w_set(neg, 0); w_set(one, 1);
  for (k = 1; k <= n; k++) {
    w_set(cur, 0);
    if (k >= 3) { w_add(cur, w[(k - 2) % 9]); w_add(cur, w[(k - 2) % 9]); }
    if (k == 2) { uint64_t f[3]; w_set(f, 4); w_add(cur, f); }
    if (k > 9) w_add(cur, w[k % 9]);
    else if (k == 9) { uint64_t f[3]; w_set(f, 9); w_add(cur, f); }
    for (i = 0; i < 3; i++) w[k % 9][i] = cur[i];
    if (n % k == 0) {
      int m = mobius_u64((uint64_t)(n / k));
      if (m != 0) {
        for (i = 0; i < 3; i++) L[i] = cur[i];
        if (k & 1) w_add(L, one); else w_sub(L, one);
        if (m > 0) w_add(pos, L); else w_add(neg, L);
      }
    }
  }
  w_sub(pos, neg);
  {
    unsigned __int128 rem = 0;
    uint64_t q[3];
    for (i = 2; i >= 0; i--) {
      unsigned __int128 cur2 = (rem << 64) | pos[i];
      q[i] = (uint64_t)(cur2 / n);
      rem = cur2 % n;
    }
    {
      unsigned __int128 u = ((unsigned __int128)q[1] << 64) | q[0];
      return u128_to_seq(u);
    }
  }
}

static seq_t a_big(idx_t n) {
  seq_t w[9];
  seq_t pos = 0, neg = 0, cur, L;
  idx_t k;
  int i;
  for (i = 0; i < 9; i++) w[i] = 0;
  for (k = 1; k <= n; k++) {
    cur = 0;
    if (k >= 3) cur = w[(k - 2) % 9] * 2;
    if (k == 2) cur += 4;
    if (k > 9) cur += w[k % 9];
    else if (k == 9) cur += 9;
    w[k % 9] = cur;
    if (n % k == 0) {
      int m = mobius_u64((uint64_t)(n / k));
      if (m != 0) {
        L = cur;
        if (k & 1) L += 1; else L -= 1;
        if (m > 0) pos += L; else neg += L;
      }
    }
  }
  return (pos - neg) / n;
}

seq_t a(idx_t n) {
  if (seq_is_fixed_width()) return a_fixed(n);
  return a_big(n);
}
