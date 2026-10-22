/*
 * A005469  a(n) = 1 + a(floor(n/2))*a(ceiling(n/2)) for n > 1, a(1) = 2.
 * https://oeis.org/A005469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(floor(n/2)) * a(ceiling(n/2)), a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)2;
    }
    
    // Ensure cache is large enough
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        // Initialize new entries to 0 (not computed)
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }
    
    // Check cache
    if (cache[n] != 0) {
        return cache[n];
    }
    
    // Compute using recurrence
    idx_t half_n = n / 2;
    idx_t ceil_n = (n + 1) / 2;
    
    seq_t result = 1 + a(half_n) * a(ceil_n);
    cache[n] = result;
    return result;
}
