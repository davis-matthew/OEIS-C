/*
 * A000570  Number of tournaments on n nodes determined by their score
 *          vectors.
 * https://oeis.org/A000570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-5) + a(n-4) + a(n-3) + a(n-1), a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=4, a(5)=7
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 86, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    static seq_t *cache = NULL;
    static idx_t cache_allocated = 0;
    static idx_t computed_up_to = 0;
    
    // Ensure cache is allocated large enough
    if (n >= cache_allocated) {
        idx_t new_allocated = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_allocated * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_allocated; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_allocated = new_allocated;
    }
    
    // Compute values up to n if not already computed
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        if (i == 1) cache[1] = 1;
        else if (i == 2) cache[2] = 1;
        else if (i == 3) cache[3] = 2;
        else if (i == 4) cache[4] = 4;
        else if (i == 5) cache[5] = 7;
        else {
            cache[i] = cache[i-5] + cache[i-4] + cache[i-3] + cache[i-1];
        }
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
