/*
 * A005685  Number of Twopins positions.
 * https://oeis.org/A005685
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 3*a(n-4) - 2*a(n-5) - a(n-8) for n
 *            >= 12, with base cases from the generating function denominator
 *            recurrence
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    // Initialize cache with base cases if needed
    if (cache == NULL) {
        cache_len = 12;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 2;
        cache[6] = 3;
        cache[7] = 5;
        cache[8] = 7;
        cache[9] = 11;
        cache[10] = 16;
        cache[11] = 26;
    }
    
    // Extend cache if needed
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i < old_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = n + 1;
        
        // Compute new values using recurrence
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = 2 * cache[i-1] - 2 * cache[i-3] + 3 * cache[i-4] - 2 * cache[i-5] - cache[i-8];
        }
    }
    
    return cache[n];
}
