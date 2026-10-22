/*
 * A005409  Number of polynomials of height n: a(1)=1, a(2)=1, a(3)=4, a(n) =
 *          2*a(n-1) + a(n-2) + 2 for n >= 4.
 * https://oeis.org/A005409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 4; a(n) = 2*a(n-1) + a(n-2) + 2 for n
 *            >= 4
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t highest_computed = 0;
    
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 4;
        highest_computed = 3;
    }
    
    if (n <= highest_computed) {
        return cache[n];
    }
    
    while (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = highest_computed + 1; i <= n; i++) {
        cache[i] = 2 * cache[i - 1] + cache[i - 2] + 2;
    }
    
    highest_computed = n;
    return cache[n];
}
