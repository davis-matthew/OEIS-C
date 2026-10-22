/*
 * A007655  Standard deviation of A007654.
 * https://oeis.org/A007655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2), a(1) = 0, a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 2;

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    
    if (cache == NULL) {
        cache_len = (n < 100000) ? 100000 : (n + 10000);
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
        cache[1] = 0;
        cache[2] = 1;
        max_computed = 2;
    }
    
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 10000 + 1));
        memcpy(new_cache, cache, sizeof(seq_t) * (cache_len + 1));
        free(cache);
        cache = new_cache;
        cache_len = n + 10000;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 14 * cache[i-1] - cache[i-2];
    }
    
    if (n > max_computed) {
        max_computed = n;
    }
    
    return cache[n];
}
