/*
 * A004001  Hofstadter-Conway $10000 sequence: a(n) = a(a(n-1)) + a(n-a(n-1))
 *          with a(1) = a(2) = 1.
 * https://oeis.org/A004001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(a(n-1)) + a(n-a(n-1)) with a(1) = a(2) = 1
 * Range:     u8 n <= 503, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 7882 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
  if (n < 1) return 0;
  
  if (n >= cache_len) {
    idx_t new_len = (n < 2) ? 3 : n + 1;
    seq_t *new_cache = mem_new(seq_t, new_len);
    
    if (cache) {
      for (idx_t i = 0; i < cache_len; i++) {
        new_cache[i] = cache[i];
      }
      mem_free(cache);
    }
    
    cache = new_cache;
    
    if (cache_len == 0) {
      cache[1] = 1;
      cache[2] = 1;
      cache_len = 3;
    }
    
    for (idx_t i = cache_len; i <= n; i++) {
      seq_t a_prev = cache[i - 1];
      idx_t idx1 = (idx_t)a_prev;
      idx_t idx2 = i - idx1;
      cache[i] = cache[idx1] + cache[idx2];
    }
    
    cache_len = new_len;
  }
  
  return cache[n];
}
