/*
 * A003424  Primes of form (p^x - 1)/(p^y - 1), p prime.
 * https://oeis.org/A003424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. A prime of the form (p^x-1)/(p^y-1) needs y | x, so it
 *            equals 1+q+...+q^(k-1) with q = p^y a prime power and k >= 2.
 *            The code lists every such value up to a bound L, keeps the
 *            primes, sorts them and removes duplicates. L is raised until
 *            there are at least n terms
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 538, u64 n <= 4738,
 *            u128 n <= 4738, big any n
 * Helpers:   A3424_cmp, A3424_isprime, A3424_isqrt, A3424_mulmod,
 *            A3424_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/A3424_cmp.h"
#include "../utils/A3424_isprime.h"
#include "../utils/A3424_isqrt.h"
#include "../utils/A3424_mulmod.h"
#include "../utils/A3424_powmod.h"

static uint64_t *A3424_terms = NULL;
static size_t A3424_cnt = 0, A3424_cap = 0;
static uint64_t A3424_L = 0;

static void A3424_push(uint64_t v) {
  if (A3424_cnt == A3424_cap) {
    size_t nc = A3424_cap ? A3424_cap * 2 : 1024;
    A3424_terms = (uint64_t *)realloc(A3424_terms, nc * sizeof(uint64_t));
    A3424_cap = nc;
  }
  A3424_terms[A3424_cnt++] = v;
}

static void A3424_build(uint64_t L) {
  uint64_t S = A3424_isqrt(L), i, j;
  int m;
  A3424_cnt = 0;
  /* k = 2: q+1 prime forces q = 2^m */
  for (m = 1; m < 63; m++) {
    uint64_t v = ((uint64_t)1 << m) + 1;
    if (v > L) break;
    if (A3424_isprime(v)) A3424_push(v);
  }
  /* k >= 3: q^2 < L, so q <= sqrt(L) */
  char *comp = (char *)calloc((size_t)S + 1, 1);
  for (i = 2; i <= S; i++) {
    if (comp[i]) continue;
    for (j = i * i; j <= S; j += i) comp[j] = 1;
    for (uint64_t q = i; q <= S; q *= i) {
      unsigned __int128 V = (unsigned __int128)1 + q;
      for (;;) {
        V = V * q + 1;
        if (V > (unsigned __int128)L) break;
        if (A3424_isprime((uint64_t)V)) A3424_push((uint64_t)V);
      }
      if (q > S / i) break;
    }
  }
  free(comp);
  qsort(A3424_terms, A3424_cnt, sizeof(uint64_t), A3424_cmp);
  size_t w = 0;
  for (size_t r = 0; r < A3424_cnt; r++)
    if (w == 0 || A3424_terms[w - 1] != A3424_terms[r]) A3424_terms[w++] = A3424_terms[r];
  A3424_cnt = w;
  A3424_L = L;
}

seq_t a(idx_t n) {
  if (A3424_L == 0) A3424_build(1000000ULL);
  while ((idx_t)A3424_cnt < n && A3424_L < ((uint64_t)1 << 61))
    A3424_build(A3424_L * 4);
  if (n < 1 || (idx_t)A3424_cnt < n) return (seq_t)0;
  return (seq_t)A3424_terms[n - 1];
}
