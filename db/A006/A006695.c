/*
 * A006695  a(2n)=2*a(2n-2)^2-1, a(2n+1)=2*a(2n)-1, a(0)=2.
 * https://oeis.org/A006695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2n) = 2*a(2n-2)^2 - 1, a(2n+1) = 2*a(2n) - 1, a(0) = 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_capacity = 100;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 2;
        cache_size = 1;
    }
    
    while (cache_size <= n) {
        if (cache_size >= cache_capacity) {
            cache_capacity *= 2;
            seq_t *new_cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
            for (idx_t i = 0; i < cache_size; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
            cache = new_cache;
        }
        
        idx_t i = cache_size;
        if (i % 2 == 1) {
            cache[i] = 2 * cache[i - 1] - 1;
        } else {
            seq_t prev = cache[i - 2];
            cache[i] = 2 * prev * prev - 1;
        }
        cache_size++;
    }
    
    return cache[n];
}
