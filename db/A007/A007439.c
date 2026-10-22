/*
 * A007439  Number of planted trees: all sub-rooted trees from any node are
 *          identical; non-root, non-leaf nodes an even distance from the root
 *          are of degree 2.
 * https://oeis.org/A007439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen, look
 * Formula:   a(n+2) = Sum a(k), k|(n), n >= 1, with a(1) = 1, a(2) = 1
 * Range:     u8 n <= 37, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (n >= cache_len) {
        idx_t new_len = (n < 1000) ? 1000 : (n + 1000);
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        
        if (cache != NULL && cache_len > 0) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (cache[n - 1] != 0) {
        return cache[n - 1];
    }
    
    if (n <= 2) {
        cache[n - 1] = 1;
        return 1;
    }
    
    seq_t sum = 0;
    idx_t m = n - 2;
    
    for (idx_t k = 1; k * k <= m; k++) {
        if (m % k == 0) {
            sum += a(k);
            if (k != m / k) {
                sum += a(m / k);
            }
        }
    }
    
    cache[n - 1] = sum;
    return sum;
}
