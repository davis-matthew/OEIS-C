/*
 * A088493  a(n) = Sum_{k=1..8} floor(p(n, k)/p(n-1, k)), where p(n, k) = n!/(
 *          Product_{i=1..floor(n/2^k)} A004001(i) ).
 * https://oeis.org/A088493
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..8} floor(p(n,k)/p(n-1,k)), p(n,k) =
 *            n!/Product_{i=1..floor(n/2^k)} A004001(i); the ratio is n if 2^k
 *            does not divide n, else floor(n/A004001(n/2^k))
 * Range:     u8 n <= 32, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 70 terms through n = 4769 (2026-11-07)
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
  seq_t sum = 0;
  for (int k = 1; k <= 8; k++) {
    idx_t d = ((idx_t)1) << k;
    if (n % d == 0) {
      seq_t v = A004001_a(n / d);
      sum += (seq_t)n / v;
    } else {
      sum += (seq_t)n;
    }
  }
  return sum;
}
