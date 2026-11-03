/*
 * A000676  Number of centered trees with n nodes.
 * https://oeis.org/A000676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = A000081(n) - [x^(n-1)] Sum_{h>=0}
 *            D_h(x)*E_{h-1}(x), where T_h = g.f. of rooted trees of height <=
 *            h (T_0 = x), E_h = Euler transform of T_h (multisets; E_{-1} =
 *            1), T_{h+1} = x*E_h, D_h = T_h - T_{h-1}. A rooted tree is
 *            centered at its root iff the maximal child heig...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 29, u64 n <= 51, u128 n <= 92,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 190 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

#define A676_CORE(NAME, T)                                                   \
static T *NAME(idx_t N) {                                                    \
  T *t = mem_new(T, N + 2);                                      \
  T *E = mem_new(T, N + 2);                                      \
  T *D = mem_new(T, N + 2);                                      \
  T *U = mem_new(T, N + 2);                                      \
  T *P = mem_new(T, N + 2);                                      \
  T *C = mem_new(T, N + 2);                                      \
  T *EV = mem_new(T, N + 2);                                     \
  T *res = mem_new(T, N + 2);                                    \
  idx_t h, i, s, m, d, k;                                                    \
  t[1] = 1; E[0] = 1; D[1] = 1;                                              \
  for (h = 0; h + 1 <= N - 1; h++) {                                         \
    idx_t lo = h + 1;                                                        \
    for (s = 0; s < lo; s++) EV[s] = 0;                                      \
    for (s = lo; s <= N - 1; s++) {                                          \
      T sum = 0;                                                             \
      for (i = lo; i <= s; i++) sum += D[i] * E[s - i];                      \
      EV[s] = sum;                                                           \
      U[s] += sum;                                                           \
    }                                                                        \
    for (k = 0; k <= N; k++) C[k] = 0;                                       \
    for (d = lo; d <= N - 1; d++)                                            \
      for (k = d; k <= N - 1; k += d) C[k] += D[d] * d;                      \
    for (m = lo; m <= N - 1; m++) {                                          \
      T sum = C[m];                                                          \
      for (i = lo; i + lo <= m; i++) sum += C[m - i] * P[i];                 \
      P[m] = sum / m;                                                        \
    }                                                                        \
    for (s = 2 * lo; s <= N - 1; s++) {                                      \
      T sum = 0;                                                             \
      for (i = 2 * lo; i <= s; i++) sum += (P[i] - D[i]) * E[s - i];         \
      EV[s] += sum;                                                          \
    }                                                                        \
    for (s = 0; s <= N - 1; s++) E[s] += EV[s];                              \
    for (m = 1; m <= N; m++) {                                               \
      if (m <= N - 1) D[m] = EV[m - 1];                                      \
      t[m] += EV[m - 1];                                                     \
    }                                                                        \
  }                                                                          \
  res[0] = 1;                                                                \
  for (m = 1; m <= N; m++) res[m] = t[m] - U[m - 1];                         \
  mem_free(t); mem_free(E); mem_free(D); mem_free(U); mem_free(P); mem_free(C); mem_free(EV);            \
  return res;                                                                \
}

A676_CORE(a676_core_seq, seq_t)
A676_CORE(a676_core_u128, unsigned __int128)

static int a676_is_fixed(void) {
  seq_t x = 1;
  int i;
  for (i = 0; i < 200; i++) x <<= 1;
  return x == 0;
}

static seq_t *a676_memo = NULL;
static idx_t a676_memoN = 0;

seq_t a(idx_t n) {
  int fixed = a676_is_fixed();
  if (n == 0) return 1;
  /* beyond this, a(n) exceeds 2^128 and cannot fit any fixed-width seq_t */
  if (fixed && n > 160) return 0;
  if (a676_memo == NULL || n > a676_memoN) {
    idx_t N = n, j;
    if (a676_memoN * 2 > N) N = a676_memoN * 2;
    if (N < 16) N = 16;
    if (fixed) {
      unsigned __int128 *r;
      if (N > 160) N = 160;
      r = a676_core_u128(N);
      if (a676_memo) mem_free(a676_memo);
      a676_memo = mem_new(seq_t, N + 2);
      for (j = 0; j <= N; j++) a676_memo[j] = u128_to_seq(r[j]);
      free(r);
    } else {
      seq_t *r = a676_core_seq(N);
      if (a676_memo) mem_free(a676_memo);
      a676_memo = r;
    }
    a676_memoN = N;
  }
  return a676_memo[n];
}
