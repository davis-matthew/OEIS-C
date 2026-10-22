/*
 * A006060  Triangular star numbers.
 * https://oeis.org/A006060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 194*a(n-1) - a(n-2) + 60, a(1) = 1, a(2) = 253
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)253;
    
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)253;
    }
    
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * n);
        memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
        free(cache);
        cache = new_cache;
        
        for (idx_t i = cache_len + 1; i <= n; i++) {
            cache[i - 1] = (seq_t)194 * cache[i - 2] - cache[i - 3] + (seq_t)60;
        }
        
        cache_len = n;
    }
    
    return cache[n - 1];
}
