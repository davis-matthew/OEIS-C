/*
 * A006164  a(n)=a(a(n-1)-4)+a(n+4-a(n-1)).
 * https://oeis.org/A006164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1)-4) + a(n+4-a(n-1)), base cases a(1)..a(6) =
 *            1,1,2,3,4,5
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t base_cases[] = {0, 1, 1, 2, 3, 4, 5};
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 6;

seq_t a(idx_t n) {
    if (n <= 6) {
        return base_cases[n];
    }
    
    if (cache == NULL) {
        cache_len = 512;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
    }
    
    while (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = 0;
        }
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t a_i_minus_1 = (i - 1 <= 6) ? base_cases[i - 1] : cache[i - 1];
        idx_t idx1 = (idx_t)(a_i_minus_1 - 4);
        idx_t idx2 = (idx_t)(i + 4 - a_i_minus_1);
        
        seq_t val1 = (idx1 <= 6) ? base_cases[idx1] : cache[idx1];
        seq_t val2 = (idx2 <= 6) ? base_cases[idx2] : cache[idx2];
        
        cache[i] = val1 + val2;
    }
    computed_up_to = n;
    
    return cache[n];
}
