/*
 * A006235  Number of spanning trees of the n-prism graph Y_n (regarding case
 *          n = 2 as a multigraph).
 * https://oeis.org/A006235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 52*a(n-3) - 35*a(n-4) + 10*a(n-5)
 *            - a(n-6), n > 5; a(0)=0, a(1)=1, a(2)=12, a(3)=75, a(4)=384,
 *            a(5)=1805
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    static const seq_t base[] = {0, 1, 12, 75, 384, 1805};
    
    if (n <= 5) {
        return base[n];
    }
    
    if (cache == NULL) {
        cache_len = 6;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 6; i++) {
            cache[i] = base[i];
        }
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = 10 * cache[i-1] 
                     - 35 * cache[i-2] 
                     + 52 * cache[i-3] 
                     - 35 * cache[i-4] 
                     + 10 * cache[i-5] 
                     - cache[i-6];
        }
    }
    
    return cache[n];
}
