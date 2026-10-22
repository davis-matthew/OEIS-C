/*
 * A007698  a(n) = 22*a(n-1) - 3*a(n-2) + 18*a(n-3) - 11*a(n-4). Deviates from
 *          A007699 at the 1403rd term.
 * https://oeis.org/A007698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 3*a(n-2) + 18*a(n-3) - 11*a(n-4), with
 *            a(1)=10, a(2)=219, a(3)=4796, a(4)=105030
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;
    
    if (n == 1) return 10;
    if (n == 2) return 219;
    if (n == 3) return 4796;
    if (n == 4) return 105030;
    
    if (n <= computed_up_to) {
        return cache[n - 1];
    }
    
    if (cache_size < n) {
        idx_t new_size = (n < 1000) ? 1000 : (n * 3 / 2);
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_size; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_size = new_size;
    }
    
    if (computed_up_to == 0) {
        cache[0] = 10;
        cache[1] = 219;
        cache[2] = 4796;
        cache[3] = 105030;
        computed_up_to = 4;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        cache[i - 1] = (seq_t)22 * cache[i - 2] - (seq_t)3 * cache[i - 3] + (seq_t)18 * cache[i - 4] - (seq_t)11 * cache[i - 5];
    }
    
    computed_up_to = n;
    return cache[n - 1];
}
