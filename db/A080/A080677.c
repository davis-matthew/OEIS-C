/*
 * A080677  a(n) = n + 1 - A004001(n).
 * https://oeis.org/A080677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 - A004001(n), where A004001(n) =
 *            A004001(A004001(n-1)) + A004001(n-A004001(n-1)), A004001(1) =
 *            A004001(2) = 1
 * Range:     u8 n <= 510, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 94 terms through n = 7916 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A004001_cache = NULL;
static idx_t A004001_cache_len = 0;

static seq_t A004001_a(idx_t n) {
  if (n < 1) return 0;

  if (n >= A004001_cache_len) {
    idx_t new_len = (n < 2) ? 3 : n + 1;
    seq_t *new_cache = mem_new(seq_t, new_len);

    if (A004001_cache) {
      for (idx_t i = 0; i < A004001_cache_len; i++) {
        new_cache[i] = A004001_cache[i];
      }
      mem_free(A004001_cache);
    }

    A004001_cache = new_cache;

    if (A004001_cache_len == 0) {
      A004001_cache[1] = 1;
      A004001_cache[2] = 1;
      A004001_cache_len = 3;
    }

    for (idx_t i = A004001_cache_len; i <= n; i++) {
      seq_t a_prev = A004001_cache[i - 1];
      idx_t idx1 = (idx_t)a_prev;
      idx_t idx2 = i - idx1;
      A004001_cache[i] = A004001_cache[idx1] + A004001_cache[idx2];
    }

    A004001_cache_len = new_len;
  }

  return A004001_cache[n];
}

seq_t a(idx_t n) {
  return (seq_t)n + 1 - A004001_a(n);
}
