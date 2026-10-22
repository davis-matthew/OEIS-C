/*
 * A007388  5th-order maximal independent sets in cycle graph.
 * https://oeis.org/A007388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = a(n-2) + a(n-7) for n >= 13. Base cases: a(1)=0, a(n)=2 for even n <= 12, a(n)=n for odd n, 3 <= n <= 12 */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_computed = 0;
    static idx_t cache_alloc = 0;
    
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n <= 12) {
        return (n % 2 == 0) ? 2 : (seq_t)n;
    }
    
    if (cache == NULL) {
        cache_alloc = 13;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        cache[1] = 0;
        cache[2] = 2;
        for (idx_t i = 3; i <= 12; i++) {
            cache[i] = (i % 2 == 0) ? 2 : (seq_t)i;
        }
        cache_computed = 13;
    }
    
    while (n >= cache_alloc) {
        cache_alloc *= 2;
        cache = (seq_t *)realloc(cache, cache_alloc * sizeof(seq_t));
    }
    
    while (cache_computed <= n) {
        cache[cache_computed] = cache[cache_computed - 2] + cache[cache_computed - 7];
        cache_computed++;
    }
    
    return cache[n];
}
