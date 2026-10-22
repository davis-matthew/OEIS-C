/*
 * A000616  a(-1)=1 by convention; for n >= 0, a(n) = number of irreducible
 *          Boolean functions of n variables.
 * https://oeis.org/A000616
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/(2^n*n!)) * Sum_{g in B_n} 2^{c(g)}, where B_n is the
 *            hyperoctahedral group acting on {0,1}^n and c(g) is the number
 *            of cycles of g on the 2^n vertices; summed over conjugacy
 *            classes (signed cycle types), a(-1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   ms_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ms_gcd.h"

#define MS_CAP 4096

typedef struct { uint64_t len[MS_CAP]; uint64_t cnt[MS_CAP]; int sz; } ms_t;

static int g_n;
static int g_mult[2][20];
static ms_t g_fac[2][20];
static ms_t g_buf[2];
static uint32_t *g_big;
static size_t g_words;
static uint64_t g_order;

static void ms_add(ms_t *m, uint64_t L, uint64_t c) {
  for (int i = 0; i < m->sz; i++)
    if (m->len[i] == L) { m->cnt[i] += c; return; }
  m->len[m->sz] = L;
  m->cnt[m->sz] = c;
  m->sz++;
}

static void ms_mul(const ms_t *x, const ms_t *y, ms_t *out) {
  out->sz = 0;
  for (int i = 0; i < x->sz; i++)
    for (int j = 0; j < y->sz; j++) {
      uint64_t g = ms_gcd(x->len[i], y->len[j]);
      ms_add(out, x->len[i] / g * y->len[j], x->cnt[i] * y->cnt[j] * g);
    }
}

static void make_factor(int k, int s, ms_t *m) {
  uint32_t size = (uint32_t)1 << k, mask = size - 1;
  bool *vis = (bool *)calloc(size, sizeof(bool));
  m->sz = 0;
  for (uint32_t st = 0; st < size; st++) {
    if (vis[st]) continue;
    uint64_t L = 0;
    uint32_t x = st;
    do {
      vis[x] = true;
      L++;
      x = ((x << 1) | (x >> (k - 1))) & mask;
      if (s) x ^= 1;
    } while (x != st);
    ms_add(m, L, 1);
  }
  free(vis);
}

static void big_add_at(size_t w, uint64_t x) {
  while (x && w < g_words) {
    uint64_t cur = (uint64_t)g_big[w] + (x & 0xffffffffULL);
    g_big[w] = (uint32_t)cur;
    x = (x >> 32) + (cur >> 32);
    w++;
  }
}

static void leaf(void) {
  int cur = 0;
  g_buf[0].sz = 1; g_buf[0].len[0] = 1; g_buf[0].cnt[0] = 1;
  uint64_t cls = g_order;
  for (int k = 1; k <= g_n; k++)
    for (int s = 0; s < 2; s++) {
      int m = g_mult[s][k];
      for (int r = 1; r <= m; r++) {
        ms_mul(&g_buf[cur], &g_fac[s][k], &g_buf[1 - cur]);
        cur = 1 - cur;
        cls /= (uint64_t)r;
        cls /= (uint64_t)(2 * k);
      }
    }
  uint64_t c = 0;
  for (int i = 0; i < g_buf[cur].sz; i++) c += g_buf[cur].cnt[i];
  size_t w = (size_t)(c / 32);
  int sh = (int)(c % 32);
  uint64_t lo = cls & 0xffffffffULL, hi = cls >> 32;
  big_add_at(w, lo << sh);
  big_add_at(w + 1, hi << sh);
}

static void rec(int k, int rem) {
  if (rem == 0) {
    for (int j = k; j <= g_n; j++) g_mult[0][j] = g_mult[1][j] = 0;
    leaf();
    return;
  }
  if (k > g_n) return;
  for (int m0 = 0; m0 * k <= rem; m0++)
    for (int m1 = 0; (m0 + m1) * k <= rem; m1++) {
      g_mult[0][k] = m0;
      g_mult[1][k] = m1;
      rec(k + 1, rem - (m0 + m1) * k);
    }
}

static void big_div(uint32_t d) {
  uint64_t r = 0;
  for (size_t i = g_words; i-- > 0;) {
    uint64_t cur = (r << 32) | g_big[i];
    g_big[i] = (uint32_t)(cur / d);
    r = cur % d;
  }
}

seq_t a(idx_t n_idx) {
  if (n_idx == -1) return (seq_t)1;
  if (n_idx < -1 || n_idx > 16) return (seq_t)0;
  int n = (int)n_idx;
  g_n = n;
  g_order = (uint64_t)1 << n;
  for (int i = 2; i <= n; i++) g_order *= (uint64_t)i;
  for (int k = 1; k <= n; k++) {
    make_factor(k, 0, &g_fac[0][k]);
    make_factor(k, 1, &g_fac[1][k]);
  }
  g_words = (((size_t)1 << n) + 64) / 32 + 4;
  g_big = (uint32_t *)calloc(g_words, sizeof(uint32_t));
  rec(1, n);
  for (int i = 0; i < n; i++) big_div(2);
  for (int i = 2; i <= n; i++) big_div((uint32_t)i);
  size_t top = g_words;
  while (top > 0 && g_big[top - 1] == 0) top--;
  seq_t res = (seq_t)0;
  for (size_t i = top; i-- > 0;)
    for (int b = 3; b >= 0; b--)
      res = res * (seq_t)256 + (seq_t)((g_big[i] >> (8 * b)) & 0xff);
  free(g_big);
  g_big = NULL;
  return res;
}
