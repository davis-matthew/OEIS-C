/*
 * A000699  Number of irreducible chord diagrams with 2n nodes.
 * https://oeis.org/A000699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*Sum_{i=1..n-1} a(i)*a(n-i) for n > 1, with a(0) =
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    
    if (n == 0 || n == 1) return 1;
    
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }
    
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *new_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
            cache = new_cache;
        }
        
        idx_t k = cache_len;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; i++) {
            sum = sum + cache[i] * cache[k - i];
        }
        cache[k] = ((seq_t)(k - 1)) * sum;
        cache_len++;
    }
    
    return cache[n];
}
