/*
 * A088359  Numbers which occur only once in A004001.
 * https://oeis.org/A088359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A051135(m) = 1, i.e. m occurs exactly
 *            once in A004001 (A004001(n) = A004001(A004001(n-1)) +
 *            A004001(n-A004001(n-1)), A004001(1) = A004001(2) = 1); computed
 *            by generating A004001 and counting runs of equal values (the
 *            sequence is nondecreasing)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9111 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *hc_seq = NULL;
static idx_t hc_len = 0, hc_cap = 0;
static seq_t *sg = NULL;
static idx_t sg_len = 0, sg_cap = 0;
static seq_t run_val = 0;
static idx_t run_len = 0;

static void hc_push_single(seq_t v) {
  if (sg_len >= sg_cap) {
    idx_t nc = sg_cap ? sg_cap * 2 : 64;
    seq_t *p = mem_new(seq_t, nc);
    for (idx_t i = 0; i < sg_len; i++) p[i] = sg[i];
    if (sg) mem_free(sg);
    sg = p;
    sg_cap = nc;
  }
  sg[sg_len++] = v;
}

static void hc_step(void) {
  if (hc_len >= hc_cap) {
    idx_t nc = hc_cap ? hc_cap * 2 : 64;
    seq_t *p = mem_new(seq_t, nc);
    for (idx_t i = 0; i < hc_len; i++) p[i] = hc_seq[i];
    if (hc_seq) mem_free(hc_seq);
    hc_seq = p;
    hc_cap = nc;
  }
  if (hc_len == 0) {
    /* index 0 unused, indices 1,2 = 1 */
    hc_seq[0] = 0;
    hc_seq[1] = 1;
    hc_seq[2] = 1;
    hc_len = 3;
    run_val = 1;
    run_len = 2;
    return;
  }
  idx_t i = hc_len;
  idx_t p = (idx_t)hc_seq[i - 1];
  seq_t v = hc_seq[p] + hc_seq[i - p];
  hc_seq[i] = v;
  hc_len++;
  if (v == run_val) {
    run_len++;
  } else {
    if (run_len == 1) hc_push_single(run_val);
    run_val = v;
    run_len = 1;
  }
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (hc_len == 0) hc_step();
  while (sg_len < n) hc_step();
  return sg[n - 1];
}
