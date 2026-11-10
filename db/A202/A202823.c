/*
 * A202823  Numbers n such that A004001(n) = A005185(n), where A004001 =
 *          Hofstadter-Conway $10000 sequence and A005185 = Hofstadter
 *          Q-sequence.
 * https://oeis.org/A202823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that A004001(n) = A005185(n); A004001(n) =
 *            A004001(A004001(n-1)) + A004001(n-A004001(n-1)),
 *            A004001(1)=A004001(2)=1; A005185(n) = A005185(n-A005185(n-1)) +
 *            A005185(n-A005185(n-2)), A005185(1)=A005185(2)=1. Computed by a
 *            incremental scan
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *hc = NULL;
static seq_t *qq = NULL;
static idx_t tlen = 0;
static idx_t *terms = NULL;
static idx_t tcount = 0;
static idx_t tcap = 0;
static idx_t scanned = 0;

static void extend_tables(idx_t newlen) {
  seq_t *nh = mem_new(seq_t, newlen);
  seq_t *nq = mem_new(seq_t, newlen);
  idx_t start;
  if (tlen == 0) {
    nh[1] = 1; nh[2] = 1;
    nq[1] = 1; nq[2] = 1;
    start = 3;
  } else {
    for (idx_t i = 0; i < tlen; i++) { nh[i] = hc[i]; nq[i] = qq[i]; }
    mem_free(hc);
    mem_free(qq);
    start = tlen;
  }
  for (idx_t i = start; i < newlen; i++) {
    idx_t p = (idx_t)nh[i - 1];
    nh[i] = nh[p] + nh[i - p];
    idx_t i1 = i - (idx_t)nq[i - 1];
    idx_t i2 = i - (idx_t)nq[i - 2];
    nq[i] = nq[i1] + nq[i2];
  }
  hc = nh;
  qq = nq;
  tlen = newlen;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  while (tcount < n) {
    idx_t newlen = (tlen == 0) ? 1024 : tlen * 2;
    extend_tables(newlen);
    for (idx_t k = (scanned < 1 ? 1 : scanned + 1); k < tlen; k++) {
      if (hc[k] == qq[k]) {
        if (tcount >= tcap) {
          idx_t nc = tcap ? tcap * 2 : 64;
          idx_t *nt = (idx_t *)malloc((size_t)nc * sizeof(idx_t));
          for (idx_t j = 0; j < tcount; j++) nt[j] = terms[j];
          if (terms) free(terms);
          terms = nt;
          tcap = nc;
        }
        terms[tcount++] = k;
      }
    }
    scanned = tlen - 1;
  }
  return (seq_t)terms[n - 1];
}
