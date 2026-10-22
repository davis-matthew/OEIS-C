/*
 * A001990  Let p be the n-th odd prime. a(n) is the least prime congruent to
 *          5 modulo 8 such that Legendre(-a(n), q) = -Legendre(-2, q) for all
 *          odd primes q <= p.
 * https://oeis.org/A001990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes a == 5 (mod 8), filtered by a CRT wheel and
 *            per-prime residue tables, for the least a with Legendre(a,q) =
 *            -Legendre(-2,q) (equivalently Legendre(-a,q) = Legendre(2,q))
 *            for all odd primes q <= p_n, where p_n is the n-th odd prime
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   a1990_isprime, a1990_mulmod, a1990_need, a1990_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1990_isprime.h"
#include "../utils/a1990_mulmod.h"
#include "../utils/a1990_need.h"
#include "../utils/a1990_powmod.h"

static uint32_t *a1990_pr = 0;
static size_t a1990_npr = 0;
static unsigned char **a1990_tab = 0;
static size_t a1990_tabcap = 0;

static void a1990_primes(size_t need) {
  uint64_t bound = 64;
  if (a1990_npr >= need) return;
  for (;;) {
    unsigned char *s = (unsigned char *)calloc((size_t)bound + 1, 1);
    size_t cnt = 0;
    uint64_t i, j;
    for (i = 2; i * i <= bound; i++)
      if (!s[i])
        for (j = i * i; j <= bound; j += i) s[j] = 1;
    for (i = 3; i <= bound; i += 2) if (!s[i]) cnt++;
    if (cnt >= need) {
      free(a1990_pr);
      a1990_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
      a1990_npr = 0;
      for (i = 3; i <= bound; i += 2) if (!s[i]) a1990_pr[a1990_npr++] = (uint32_t)i;
      free(s);
      break;
    }
    free(s);
    bound *= 2;
  }
  if (a1990_tabcap < a1990_npr) {
    unsigned char **t = (unsigned char **)calloc(a1990_npr, sizeof(unsigned char *));
    if (a1990_tab) {
      memcpy(t, a1990_tab, a1990_tabcap * sizeof(unsigned char *));
      free(a1990_tab);
    }
    a1990_tab = t;
    a1990_tabcap = a1990_npr;
  }
}

static unsigned char *a1990_table(size_t i) {
  if (!a1990_tab[i]) {
    uint32_t q = a1990_pr[i], x;
    unsigned char *sq = (unsigned char *)calloc(q, 1);
    unsigned char *t = (unsigned char *)calloc(q, 1);
    int need = a1990_need(q);
    for (x = 1; x < q; x++) sq[(uint64_t)x * x % q] = 1;
    for (x = 1; x < q; x++) t[x] = (sq[x] == need) ? 1 : 0;
    free(sq);
    a1990_tab[i] = t;
  }
  return a1990_tab[i];
}

static bool a1990_ok(uint64_t c, size_t i) {
  uint32_t q = a1990_pr[i];
  if (q < 8192) return a1990_table(i)[c % q] != 0;
  {
    uint64_t r = c % q, l;
    if (r == 0) return false;
    l = a1990_powmod(r, (q - 1) / 2, q);
    return (l == 1) == (a1990_need(q) == 1);
  }
}

static uint64_t *a1990_wheel = 0;
static size_t a1990_wn = 0;
static size_t a1990_ww = 0;
static uint64_t a1990_M = 0;

static void a1990_build_wheel(size_t w) {
  uint64_t M = 8, r;
  size_t i, cap = 16;
  if (a1990_wheel && a1990_ww == w) return;
  for (i = 0; i < w; i++) M *= a1990_pr[i];
  free(a1990_wheel);
  a1990_wheel = (uint64_t *)malloc(cap * sizeof(uint64_t));
  a1990_wn = 0;
  for (r = 5; r < M; r += 8) {
    bool good = true;
    for (i = 0; i < w && good; i++) good = a1990_ok(r, i);
    if (good) {
      if (a1990_wn == cap) {
        cap *= 2;
        a1990_wheel = (uint64_t *)realloc(a1990_wheel, cap * sizeof(uint64_t));
      }
      a1990_wheel[a1990_wn++] = r;
    }
  }
  a1990_ww = w;
  a1990_M = M;
}

#define A1990_CACHE 4096
static uint64_t a1990_cache[A1990_CACHE];

seq_t a(idx_t n) {
  size_t np = (size_t)n, w, i, j;
  uint64_t lo = 0, k;
  idx_t m;
  if (n < A1990_CACHE && a1990_cache[n]) return (seq_t)a1990_cache[n];
  for (m = (n < A1990_CACHE ? n : A1990_CACHE); m > 1; m--) {
    if (a1990_cache[m - 1]) { lo = a1990_cache[m - 1]; break; }
  }
  a1990_primes(np);
  w = np < 7 ? np : 7; /* wheel primes 3..19 */
  a1990_build_wheel(w);
  for (k = lo / a1990_M;; k++) {
    for (j = 0; j < a1990_wn; j++) {
      uint64_t c = k * a1990_M + a1990_wheel[j];
      bool good = true;
      if (c < lo) continue;
      for (i = w; i < np && good; i++) good = a1990_ok(c, i);
      if (good && a1990_isprime(c)) {
        if (n < A1990_CACHE) a1990_cache[n] = c;
        return (seq_t)c;
      }
    }
  }
}
