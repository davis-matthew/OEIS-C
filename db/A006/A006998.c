/*
 * A006998  Partitioning integers to avoid arithmetic progressions of length
 *          3.
 * https://oeis.org/A006998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(floor(2n/3)) + a(floor((2n+1)/3)), with a(0) = 0, a(1)
 *            = 1, a(2) = 2
 * Range:     u8 n <= 40, u16 n <= 1064, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    
    if (n < cache_len && computed[n]) {
        return cache[n];
    }
    
    seq_t result = a((2 * n) / 3) + a((2 * n + 1) / 3);
    
    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len : 16;
        while (new_len <= n) {
            new_len *= 2;
        }
        
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        bool *new_computed = (bool *)realloc(computed, new_len * sizeof(bool));
        
        if (!new_cache || !new_computed) {
            free(new_cache);
            free(new_computed);
            return result;
        }
        
        idx_t old_len = cache_len;
        cache = new_cache;
        computed = new_computed;
        cache_len = new_len;
        
        memset(cache + old_len, 0, (new_len - old_len) * sizeof(seq_t));
        memset(computed + old_len, 0, (new_len - old_len) * sizeof(bool));
    }
    
    cache[n] = result;
    computed[n] = true;
    return result;
}
