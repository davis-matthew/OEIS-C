/*
 * A007392  Number of strict 3rd-order maximal independent sets in cycle
 *          graph.
 * https://oeis.org/A007392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-5) + a(n-6) - 2*a(n-7) +
 *            a(n-9), with initial values a(1) through a(12)
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t highest_computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n <= 9) return 0;
    if (n == 10) return 5;
    if (n == 11) return 0;
    if (n == 12) return 12;
    
    // For n >= 13
    if (cache == NULL) {
        // Initialize cache
        idx_t initial_size = (n + 1) < 1000 ? 1000 : (n + 1) * 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * initial_size);
        cache_size = initial_size;
        
        // Initialize first 13 elements
        cache[1] = 0; cache[2] = 0; cache[3] = 0; cache[4] = 0; cache[5] = 0;
        cache[6] = 0; cache[7] = 0; cache[8] = 0; cache[9] = 0; cache[10] = 5;
        cache[11] = 0; cache[12] = 12;
        
        highest_computed = 12;
    } else if (n >= cache_size) {
        // Resize cache
        idx_t new_size = n * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Compute from highest_computed + 1 to n
    for (idx_t i = highest_computed + 1; i <= n; i++) {
        cache[i] = 3 * cache[i-2] - 3 * cache[i-4] + cache[i-5] + cache[i-6] - 2 * cache[i-7] + cache[i-9];
    }
    highest_computed = n;
    
    return cache[n];
}
