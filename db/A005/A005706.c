/*
 * A005706  Number of partitions of 5n into powers of 5.
 * https://oeis.org/A005706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n/5), a(0) = 1
 * Range:     u8 n <= 42, u16 n <= 322, u32 n <= 5105, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    // Initialize cache if needed
    if (cache == NULL) {
        cache_size = 1024;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        memset(cache, 0, cache_size * sizeof(seq_t));
        cache[0] = 1;
    }
    
    // Extend cache if needed
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Return cached value if already computed
    if (cache[n] != 0) {
        return cache[n];
    }
    
    // Compute iteratively from 1 to n
    for (idx_t i = 1; i <= n; i++) {
        if (cache[i] == 0) {
            cache[i] = cache[i - 1] + cache[i / 5];
        }
    }
    
    return cache[n];
}
