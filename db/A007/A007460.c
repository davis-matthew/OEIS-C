/*
 * A007460  Shifts left under OR-convolution with itself.
 * https://oeis.org/A007460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n) = sum_{k=0}^{n-1} (a(k) | a(n-1-k)), a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
  static seq_t *cache = NULL;
  static idx_t cache_size = 0;
  
  if (cache == NULL) {
    cache = (seq_t *)malloc(sizeof(seq_t));
    cache[0] = 1;
    cache_size = 1;
  }
  
  while (cache_size <= n) {
    idx_t i = cache_size;
    seq_t sum = 0;
    for (idx_t k = 0; k < i; k++) {
      sum += cache[k] | cache[i - 1 - k];
    }
    cache = (seq_t *)realloc(cache, (i + 1) * sizeof(seq_t));
    cache[i] = sum;
    cache_size++;
  }
  
  return cache[n];
}
