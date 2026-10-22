/*
 * A001883  Number of permutations s of {1,2,...,n} such that |s(i)-i|>1 for
 *          each i=1,2,...,n.
 * https://oeis.org/A001883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 4*a(n-2) - 3*(n-3)*a(n-3) + (n-4)*a(n-4) +
 *            2*(n-5)*a(n-5) - (n-7)*a(n-6) - a(n-7), a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=1, a(5)=4, a(6)=29
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    // Ensure cache is large enough
    if (cache_cap < 7 || n >= cache_cap) {
        idx_t new_cap = (n >= 6) ? (n + 1) : 7;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        
        if (cache != NULL && cache_computed > 0) {
            memcpy(new_cache, cache, cache_computed * sizeof(seq_t));
            free(cache);
        }
        
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    // Initialize base cases if not done
    if (cache_computed == 0) {
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 4;
        cache[6] = 29;
        cache_computed = 7;
    }
    
    // Compute values up to n using recurrence relation
    while (cache_computed <= n) {
        idx_t i = cache_computed;
        seq_t val = (seq_t)i * cache[i-1] 
                  + 4 * cache[i-2]
                  - 3 * (seq_t)(i-3) * cache[i-3]
                  + (seq_t)(i-4) * cache[i-4]
                  + 2 * (seq_t)(i-5) * cache[i-5]
                  - (seq_t)(i-7) * cache[i-6]
                  - cache[i-7];
        cache[i] = val;
        cache_computed++;
    }
    
    return cache[n];
}
