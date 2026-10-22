/*
 * A007384  Number of strict 3rd-order maximal independent sets in path graph.
 * https://oeis.org/A007384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-5) + a(n-6) - 2*a(n-7) +
 *            a(n-9), with a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=0,
 *            a(7)=3, a(8)=0, a(9)=6
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t base_values[] = {0, 0, 0, 0, 1, 0, 3, 0, 6};
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    // Base cases
    if (n <= 9) {
        return base_values[n - 1];
    }
    
    // Return cached value if available
    if (computed_up_to >= n) {
        return cache[n];
    }
    
    // Allocate cache if needed
    if (cache_size == 0) {
        cache_size = (n < 10000) ? 10001 : n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        
        // Initialize base cases
        for (idx_t i = 1; i <= 9; i++) {
            cache[i] = base_values[i - 1];
        }
        computed_up_to = 9;
    } else if (cache_size <= n) {
        // Extend cache
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Compute up to n
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t val = (seq_t)3 * cache[i-2]
                  - (seq_t)3 * cache[i-4]
                  + cache[i-5]
                  + cache[i-6]
                  - (seq_t)2 * cache[i-7]
                  + cache[i-9];
        cache[i] = val;
    }
    
    computed_up_to = n;
    return cache[n];
}
