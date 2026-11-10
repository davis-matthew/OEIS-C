/*
 * A094528  Triangle T(n,k) read by rows related to A004001.
 * https://oeis.org/A094528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A004001(2^k+n) - 2^(k-1), 1<=k<=n, read by rows; a(m) =
 *            T(n,k) with m = n(n-1)/2 + k. A004001: b(n) = b(b(n-1)) +
 *            b(n-b(n-1)), b(1) = b(2) = 1
 * Range:     u8 n <= 44, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

seq_t a(idx_t m) {
  if (m < 1) return 0;
  /* find row n: smallest n with n(n+1)/2 >= m */
  idx_t n = 1;
  idx_t start = 0; /* n(n-1)/2 */
  while (start + n < m) {
    start += n;
    n++;
  }
  idx_t k = m - start;
  if (k >= 40) return 0; /* index 2^k+n is infeasible to tabulate */
  idx_t big = ((idx_t)1 << k) + n;
  seq_t half = (seq_t)1;
  for (idx_t i = 1; i < k; i++) half = half * 2;
  return A004001_a(big) - half;
}
