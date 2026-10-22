/*
 * A006980  Compositions: 6th column of A048004.
 * https://oeis.org/A006980
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - a(n-6) + a(n-7) + 2*a(n-8) -
 *            a(n-13), a(6) = 1, a(7) = 2, a(8) = 5
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n < 6) return 0;
    if (n == 6) return 1;
    if (n == 7) return 2;
    if (n == 8) return 5;
    
    // Ensure cache is large enough
    if (cache_size <= n) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        if (new_size < 1024) new_size = 1024;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        if (cache != NULL) {
            memcpy(new_cache, cache, old_size * sizeof(seq_t));
            free(cache);
        }
        
        // Initialize new portion to 0
        memset(new_cache + old_size, 0, (new_size - old_size) * sizeof(seq_t));
        
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Set base cases if not already set
    if (cache[6] == 0) {
        cache[6] = 1;
        cache[7] = 2;
        cache[8] = 5;
    }
    
    // Compute values up to n using recurrence
    for (idx_t i = 9; i <= n; i++) {
        if (cache[i] != 0) continue;  // Already computed
        
        seq_t val = (seq_t)4 * cache[i-1] - (seq_t)4 * cache[i-2];
        if (i >= 6) val -= cache[i-6];
        if (i >= 7) val += cache[i-7];
        if (i >= 8) val += (seq_t)2 * cache[i-8];
        if (i >= 13) val -= cache[i-13];
        cache[i] = val;
    }
    
    return cache[n];
}
