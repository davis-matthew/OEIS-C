/*
 * A002789  Number of integer points in a certain quadrilateral scaled by a
 *          factor of n.
 * https://oeis.org/A002789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6) with a(1)=2,
 *            a(2)=4, a(3)=7, a(4)=11, a(5)=16, a(6)=21
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
  if (n <= 0) return 0;
  
  static seq_t *cache = NULL;
  static idx_t cache_len = 0;
  
  if (cache == NULL) {
    cache = (seq_t *)malloc(6 * sizeof(seq_t));
    cache[0] = 2;
    cache[1] = 4;
    cache[2] = 7;
    cache[3] = 11;
    cache[4] = 16;
    cache[5] = 21;
    cache_len = 6;
  }
  
  if (n <= cache_len) {
    return cache[n - 1];
  }
  
  cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
  
  for (idx_t i = cache_len; i < n; i++) {
    cache[i] = cache[i - 1] + cache[i - 2] - cache[i - 4] - cache[i - 5] + cache[i - 6];
  }
  
  cache_len = n;
  return cache[n - 1];
}
