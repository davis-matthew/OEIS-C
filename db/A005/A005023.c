/*
 * A005023  Number of walks of length 2n+7 in the path graph P_8 from one end
 *          to the other.
 * https://oeis.org/A005023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - a(n-4), with a(1)=7,
 *            a(2)=34, a(3)=143, a(4)=560
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    static const seq_t base[] = {0, 7, 34, 143, 560};
    
    if (n < 5) {
        return (n > 0) ? base[n] : 0;
    }
    
    if (max_computed >= n) {
        return cache[n];
    }
    
    if (cache == NULL) {
        idx_t new_len = (n < 100) ? 100 : (n + 1) * 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (cache == NULL) return 0;
        cache_len = new_len;
        
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache[4] = base[4];
        max_computed = 4;
    } else if (cache_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (new_cache == NULL) return 0;
        
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 7 * cache[i-1] - 15 * cache[i-2] + 10 * cache[i-3] - cache[i-4];
    }
    
    max_computed = n;
    return cache[n];
}
