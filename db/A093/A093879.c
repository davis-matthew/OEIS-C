/*
 * A093879  First differences of A004001.
 * https://oeis.org/A093879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A004001(n+1) - A004001(n), where A004001(n) =
 *            A004001(A004001(n-1)) + A004001(n-A004001(n-1)), A004001(1) =
 *            A004001(2) = 1
 * Range:     u8 n <= 1031, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 121 terms through n = 9344 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A093879_cache = NULL;
static idx_t A093879_len = 0; /* highest index filled */
static idx_t A093879_cap = 0;

static void A093879_extend(idx_t n) {
  if (n < 2) n = 2;
  if (n >= A093879_cap) {
    idx_t new_cap = n + 1;
    if (new_cap < 2 * A093879_cap) new_cap = 2 * A093879_cap;
    if (new_cap < 8) new_cap = 8;
    seq_t *nc = mem_new(seq_t, new_cap);
    if (A093879_cache) {
      for (idx_t i = 0; i <= A093879_len; i++) nc[i] = A093879_cache[i];
      mem_free(A093879_cache);
    }
    A093879_cache = nc;
    A093879_cap = new_cap;
  }
  if (A093879_len < 2) {
    A093879_cache[1] = 1;
    A093879_cache[2] = 1;
    A093879_len = 2;
  }
  for (idx_t i = A093879_len + 1; i <= n; i++) {
    idx_t p = (idx_t)A093879_cache[i - 1];
    A093879_cache[i] = A093879_cache[p] + A093879_cache[i - p];
  }
  if (n > A093879_len) A093879_len = n;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  A093879_extend(n + 1);
  return A093879_cache[n + 1] - A093879_cache[n];
}
