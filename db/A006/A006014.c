/*
 * A006014  a(n+1) = (n+1)*a(n) + Sum_{k=1..n-1} a(k)*a(n-k).
 * https://oeis.org/A006014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = (n+1)*a(n) + Sum_{k=1..n-1} a(k)*a(n-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
  if (n < 1) return 0;
  
  // Ensure cache is large enough
  if (cache_len < n) {
    idx_t old_len = cache_len;
    idx_t new_len = (cache_len == 0) ? 10 : (cache_len * 2);
    if (new_len < n) new_len = n + 10;
    
    seq_t *new_cache = (seq_t *)calloc(new_len + 1, sizeof(seq_t));
    
    if (cache != NULL) {
      for (idx_t i = 0; i <= old_len; i++) {
        new_cache[i] = cache[i];
      }
      free(cache);
    }
    
    cache = new_cache;
    cache_len = new_len;
  }
  
  // If already computed, return
  if (cache[n] != 0) return cache[n];
  
  // Base cases
  if (n == 1) {
    cache[1] = 1;
    return 1;
  }
  if (n == 2) {
    cache[2] = 2;
    return 2;
  }
  
  // Compute: a(n) = n * a(n-1) + Sum_{k=1..n-2} a(k)*a(n-1-k)
  seq_t result = (seq_t)n * a(n - 1);
  
  for (idx_t k = 1; k < n - 1; k++) {
    result += a(k) * a(n - 1 - k);
  }
  
  cache[n] = result;
  return result;
}
