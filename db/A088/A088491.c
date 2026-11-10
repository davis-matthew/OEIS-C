/*
 * A088491  a(n) = floor(p(n)/p(n-1)), where p(n) =
 *          n!/(Product_{j=1..floor(n/2)} A004001(j)).
 * https://oeis.org/A088491
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(p(n)/p(n-1)) = n if n odd, floor(n/A004001(n/2)) if
 *            n even; A004001(n) = A004001(A004001(n-1)) +
 *            A004001(n-A004001(n-1)), A004001(1)=A004001(2)=1
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 102 terms through n = 4863 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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
  if (n & 1) return (seq_t)n;
  seq_t h = A004001_a(n / 2);
  return (seq_t)n / h;
}
