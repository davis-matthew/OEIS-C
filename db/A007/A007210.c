/*
 * A007210  Oscillates under partition transform.
 * https://oeis.org/A007210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of partitions of n into parts from {b(k)}, and
 *            b(n) = A007211(n) = number of partitions of n into parts from
 *            {a(k)}, with a(1..3) = b(1..3) = 1, 2, 3. Both are computed
 *            together, adding each new term as an allowed part (a coin-change
 *            DP)
 * Range:     u8 n <= 21, u16 n <= 72, u32 n <= 315, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *SA = 0, *SB = 0, *PA = 0, *PB = 0;
static idx_t cap = 0;

static seq_t sat_add(seq_t x, seq_t y) {
  seq_t s = x + y;
  if (s < x) s = (seq_t)0 - 1; /* saturate (fixed-width only) */
  return s;
}

static void add_part(seq_t *P, idx_t p, idx_t N) {
  for (idx_t m = p; m <= N; m++) P[m] = sat_add(P[m], P[m - p]);
}

static void build(idx_t N) {
  if (SA) {
    release(SA, cap + 1);
    release(SB, cap + 1);
    release(PA, cap + 1);
    release(PB, cap + 1);
  }
  SA = alloc_zero(N + 1);
  SB = alloc_zero(N + 1);
  PA = alloc_zero(N + 1); /* partitions into parts from a */
  PB = alloc_zero(N + 1); /* partitions into parts from b */
  PA[0] = 1;
  PB[0] = 1;
  idx_t ia = 1, ib = 1;
  for (idx_t n = 1; n <= N; n++) {
    while (ia < n && SA[ia] >= 1 && SA[ia] <= n) {
      if (ia == 1 || SA[ia] != SA[ia - 1])
        add_part(PA, (idx_t)(long long)SA[ia], N);
      ia++;
    }
    while (ib < n && SB[ib] >= 1 && SB[ib] <= n) {
      if (ib == 1 || SB[ib] != SB[ib - 1])
        add_part(PB, (idx_t)(long long)SB[ib], N);
      ib++;
    }
    if (n <= 3) {
      SA[n] = (seq_t)n;
      SB[n] = (seq_t)n;
      continue;
    }
    seq_t x = PB[n], y = PA[n];
    if (x == n) y = sat_add(y, 1);
    if (y == n) x = sat_add(x, 1);
    SA[n] = x;
    SB[n] = y;
  }
  cap = N;
}

seq_t a(idx_t n) {
  if (n < 1) n = 1;
  if (n > cap) {
    idx_t N = cap * 2;
    if (N < n) N = n;
    if (N < 16) N = 16;
    build(N);
  }
  return SA[n];
}
