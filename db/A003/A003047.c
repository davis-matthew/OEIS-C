/*
 * A003047  a(n) = Catalan(n) * Product_{k = 0..n-1} a(k).
 * https://oeis.org/A003047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((4*n-6)*a(n-1)^2)/n for n > 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
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
    if (n <= 0) return 0;
    
    if (cache == NULL) {
        cache_len = 128;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
    }
    
    while (n > cache_len) {
        idx_t newlen = cache_len * 2;
        seq_t *newcache = (seq_t *)malloc(newlen * sizeof(seq_t));
        memcpy(newcache, cache, cache_len * sizeof(seq_t));
        memset((char *)newcache + cache_len * sizeof(seq_t), 0,
               (newlen - cache_len) * sizeof(seq_t));
        free(cache);
        cache = newcache;
        cache_len = newlen;
    }
    
    if (cache[n - 1] != 0) {
        return cache[n - 1];
    }
    
    if (n == 1 || n == 2) {
        cache[n - 1] = 1;
        return 1;
    }
    
    seq_t prev = a(n - 1);
    seq_t numerator = ((seq_t)(4 * n - 6)) * prev * prev;
    seq_t result = numerator / ((seq_t)n);
    
    cache[n - 1] = result;
    return result;
}
