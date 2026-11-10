/*
 * A059983  First occurrence of n in A007599.
 * https://oeis.org/A059983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A007599(k) = n, where A007599(0)=1,
 *            A007599(1)=0, A007599(k) = A007599(k-1) +
 *            A007599^(floor(k/2))(k-2) (iterated composition); computed by
 *            generating A007599 and scanning for first occurrence
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9293 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A007599_v = NULL;
static idx_t A007599_len = 0;
static idx_t A007599_cap = 0;

static void A007599_extend(void) {
  if (A007599_v == NULL) {
    A007599_cap = 1024;
    A007599_v = mem_new(seq_t, A007599_cap);
    A007599_v[0] = 1;
    A007599_v[1] = 0;
    A007599_len = 2;
  }
  if (A007599_len >= A007599_cap) {
    idx_t nc = A007599_cap * 2;
    A007599_v = mem_resize(seq_t, A007599_v, nc);
    A007599_cap = nc;
  }
  idx_t k = A007599_len;
  idx_t depth = k / 2;
  seq_t val = (seq_t)(k - 2);
  for (idx_t d = 0; d < depth; d++) {
    if (val == 0) {
      idx_t rem = depth - d;
      val = (rem % 2 == 1) ? 1 : 0;
      break;
    } else if (val == 1) {
      idx_t rem = depth - d;
      val = (rem % 2 == 1) ? 0 : 1;
      break;
    }
    val = A007599_v[(idx_t)val];
  }
  A007599_v[k] = A007599_v[k - 1] + val;
  A007599_len = k + 1;
}

seq_t a(idx_t n) {
  if (n == 0) return 1;
  if (n == 1) return 0;
  if (A007599_v == NULL) A007599_extend();
  seq_t target = (seq_t)n;
  while (A007599_v[A007599_len - 1] < target) A007599_extend();
  for (idx_t k = 2; k < A007599_len; k++) {
    if (A007599_v[k] == target) return (seq_t)k;
    if (A007599_v[k] > target) break;
  }
  return 0;
}
