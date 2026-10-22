/*
 * A005619  Number of Boolean functions realized by n-input cascades.
 * https://oeis.org/A005619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*a(n-1) - 80*a(n-2) + 48, a(0) = 2, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 17,
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
    if (n == 0) return 2;
    if (n == 1) return 4;
    
    if (cache == NULL) {
        cache_len = (n + 2) * 2;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[0] = 2;
        cache[1] = 4;
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = (n + 2) * 2;
        seq_t *new_cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        memcpy(new_cache, cache, old_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }
    
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)32 * cache[i-1] - (seq_t)80 * cache[i-2] + (seq_t)48;
        }
    }
    
    return cache[n];
}
