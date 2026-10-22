/*
 * A007542  Successive integers produced by Conway's PRIMEGAME.
 * https://oeis.org/A007542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look, nice
 * Formula:   a(n+1) = A203907(a(n)), a(0) = 2: iterate Conway's PRIMEGAME
 *            (fractions 17/91, 78/85, 19/51, 23/38, 29/33, 77/29, 95/23,
 *            77/19, 1/17, 11/13, 13/11, 15/2, 1/7, 55/1). The state is kept
 *            as a vector of prime exponents so intermediate terms never
 *            overflow; a(n) is built at the end
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 422, u64 n <= 2881,
 *            u128 n <= 8102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define NP 10
static const unsigned primes[NP] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

/* exponent deltas for each fraction, primes 2,3,5,7,11,13,17,19,23,29 */
static const int fr[14][NP] = {
  /* 17/91 */ {0, 0, 0, -1, 0, -1, 1, 0, 0, 0},
  /* 78/85 */ {1, 1, -1, 0, 0, 1, -1, 0, 0, 0},
  /* 19/51 */ {0, -1, 0, 0, 0, 0, -1, 1, 0, 0},
  /* 23/38 */ {-1, 0, 0, 0, 0, 0, 0, -1, 1, 0},
  /* 29/33 */ {0, -1, 0, 0, -1, 0, 0, 0, 0, 1},
  /* 77/29 */ {0, 0, 0, 1, 1, 0, 0, 0, 0, -1},
  /* 95/23 */ {0, 0, 1, 0, 0, 0, 0, 1, -1, 0},
  /* 77/19 */ {0, 0, 0, 1, 1, 0, 0, -1, 0, 0},
  /* 1/17  */ {0, 0, 0, 0, 0, 0, -1, 0, 0, 0},
  /* 11/13 */ {0, 0, 0, 0, 1, -1, 0, 0, 0, 0},
  /* 13/11 */ {0, 0, 0, 0, -1, 1, 0, 0, 0, 0},
  /* 15/2  */ {-1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  /* 1/7   */ {0, 0, 0, -1, 0, 0, 0, 0, 0, 0},
  /* 55/1  */ {0, 0, 1, 0, 1, 0, 0, 0, 0, 0}
};

static void step(const int *cur, int *nxt) {
  int i, j;
  for (i = 0; i < 14; i++) {
    int ok = 1;
    for (j = 0; j < NP; j++)
      if (cur[j] + fr[i][j] < 0) { ok = 0; break; }
    if (ok) {
      for (j = 0; j < NP; j++) nxt[j] = cur[j] + fr[i][j];
      return;
    }
  }
}

static int *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_cnt = 0;

seq_t a(idx_t n) {
  seq_t r;
  int j, e;
  int *v;
  if (cache == NULL) {
    cache_cap = 1024;
    cache = (int *)malloc((size_t)cache_cap * NP * sizeof(int));
    for (j = 0; j < NP; j++) cache[j] = 0;
    cache[0] = 1;
    cache_cnt = 1;
  }
  if (n >= cache_cap) {
    idx_t newcap = cache_cap;
    while (n >= newcap) newcap *= 2;
    cache = (int *)realloc(cache, (size_t)newcap * NP * sizeof(int));
    cache_cap = newcap;
  }
  while (cache_cnt <= n) {
    step(cache + (size_t)(cache_cnt - 1) * NP, cache + (size_t)cache_cnt * NP);
    cache_cnt++;
  }
  v = cache + (size_t)n * NP;
  r = 1;
  for (j = 0; j < NP; j++)
    for (e = 0; e < v[j]; e++) r *= primes[j];
  return r;
}
