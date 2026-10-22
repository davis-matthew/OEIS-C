/*
 * A007385  Number of strict 5th-order maximal independent sets in path graph.
 * https://oeis.org/A007385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) + a(n-7) - 2*a(n-9) +
 *            a(n-11)
 * Range:     u8 n <= 32, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=1, a(8)=0, a(9)=3, a(10)=0, a(11)=6 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_allocated = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    static const seq_t initial[] = {0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 6};
    
    if (n <= 11) {
        return initial[n - 1];
    }
    
    if (cache == NULL) {
        cache_allocated = (n < 100) ? 100 : (n + 100);
        cache = (seq_t *)malloc(cache_allocated * sizeof(seq_t));
        if (cache == NULL) return 0;
        
        for (idx_t i = 0; i < 11; i++) {
            cache[i] = initial[i];
        }
        cache_computed = 11;
    }
    
    while (cache_allocated <= n) {
        idx_t new_allocated = cache_allocated * 2;
        seq_t *new_cache = (seq_t *)malloc(new_allocated * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        
        memcpy(new_cache, cache, cache_allocated * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_allocated = new_allocated;
    }
    
    for (idx_t i = cache_computed + 1; i <= n; i++) {
        seq_t value = (seq_t)3 * cache[i - 3]
                    - (seq_t)3 * cache[i - 5]
                    + cache[i - 7]
                    + cache[i - 8]
                    - (seq_t)2 * cache[i - 10]
                    + cache[i - 12];
        cache[i - 1] = value;
    }
    
    cache_computed = n;
    return cache[n - 1];
}
