/*
 * A002526  Number of permutations of length n within distance 3 of a fixed
 *          permutation.
 * https://oeis.org/A002526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 10*a(n-4) + 8*a(n-5) - 2*a(n-6) -
 *            16*a(n-7) - 10*a(n-8) - 2*a(n-9) + 4*a(n-10) + 2*a(n-11) +
 *            2*a(n-13) + a(n-14), with initial values a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=6, a(4)=24, a(5)=78, a(6)=230, a(7)=675, a(8)=2069,
 *            a(9)=6404, a(10)=19708, a(11)=60216, a(12)=183988, a(...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void init_cache(void) {
    if (cache_cap > 0) return;
    
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)24;
    cache[5] = (seq_t)78;
    cache[6] = (seq_t)230;
    cache[7] = (seq_t)675;
    cache[8] = (seq_t)2069;
    cache[9] = (seq_t)6404;
    cache[10] = (seq_t)19708;
    cache[11] = (seq_t)60216;
    cache[12] = (seq_t)183988;
    cache[13] = (seq_t)563172;
    cache_len = 14;
}

seq_t a(idx_t n) {
    init_cache();
    
    if (n < cache_len) {
        return cache[n];
    }
    
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        
        idx_t i = cache_len;
        seq_t val = (seq_t)2 * cache[i-1]
                  + (seq_t)2 * cache[i-2]
                  + (seq_t)10 * cache[i-4]
                  + (seq_t)8 * cache[i-5]
                  - (seq_t)2 * cache[i-6]
                  - (seq_t)16 * cache[i-7]
                  - (seq_t)10 * cache[i-8]
                  - (seq_t)2 * cache[i-9]
                  + (seq_t)4 * cache[i-10]
                  + (seq_t)2 * cache[i-11]
                  + (seq_t)2 * cache[i-13]
                  + (seq_t)1 * cache[i-14];
        
        cache[cache_len++] = val;
    }
    
    return cache[n];
}
