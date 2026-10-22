/*
 * A001251  Number of permutations of order n with the length of longest run
 *          equal to 3.
 * https://oeis.org/A001251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n) - 2*E(n), computed by an O(n^2) dynamic program over
 *            permutations. B(n) counts permutations of [n] with no three
 *            consecutive ascents and no three consecutive descents (every
 *            monotone run has at most 3 elements). 2*E(n) counts the
 *            alternating permutations, where every run has exactly 2...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   zal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zal.h"

static seq_t *res = 0;
static idx_t rescap = 0; /* res[1..rescap] valid */

static void compute(idx_t N) {
  seq_t *r = zal(N + 1);
  idx_t L = N + 2;
  seq_t *U1 = zal(L), *U2 = zal(L), *D1 = zal(L), *D2 = zal(L);
  seq_t *AU = zal(L), *AD = zal(L);
  seq_t *nU1 = zal(L), *nU2 = zal(L), *nD1 = zal(L), *nD2 = zal(L);
  seq_t *nAU = zal(L), *nAD = zal(L);
  seq_t *tmp;
  idx_t i, k;

  r[1] = 0;
  if (N >= 2) r[2] = 0;
  U1[2] = 1; D1[1] = 1;
  AU[2] = 1; AD[1] = 1;

  for (i = 2; i < N; i++) {
    seq_t s1 = 0, s2 = 0, sa = 0, t1 = 0, t2 = 0, ta = 0, tot = 0, alt = 0;
    for (k = 1; k <= i + 1; k++) {
      if (k >= 2) {
        s1 += D1[k - 1];
        s1 += D2[k - 1];
        s2 += U1[k - 1];
        sa += AD[k - 1];
      }
      nU1[k] = s1;
      nU2[k] = s2;
      nAU[k] = sa;
    }
    for (k = i + 1; k >= 1; k--) {
      if (k <= i) {
        t1 += U1[k];
        t1 += U2[k];
        t2 += D1[k];
        ta += AU[k];
      }
      nD1[k] = t1;
      nD2[k] = t2;
      nAD[k] = ta;
    }
    for (k = 1; k <= i + 1; k++) {
      tot += nU1[k];
      tot += nU2[k];
      tot += nD1[k];
      tot += nD2[k];
      alt += nAU[k];
      alt += nAD[k];
    }
    r[i + 1] = tot - alt;

    tmp = U1; U1 = nU1; nU1 = tmp;
    tmp = U2; U2 = nU2; nU2 = tmp;
    tmp = D1; D1 = nD1; nD1 = tmp;
    tmp = D2; D2 = nD2; nD2 = tmp;
    tmp = AU; AU = nAU; nAU = tmp;
    tmp = AD; AD = nAD; nAD = tmp;
  }

  free(U1); free(U2); free(D1); free(D2); free(AU); free(AD);
  free(nU1); free(nU2); free(nD1); free(nD2); free(nAU); free(nAD);
  if (res) free(res);
  res = r;
  rescap = N;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (n > rescap) {
    idx_t N = n;
    if (N < 2 * rescap) N = 2 * rescap;
    if (N < 64) N = 64;
    compute(N);
  }
  return res[n];
}
