/*
 * A006847  Number of extreme points of the set of n X n symmetric
 *          doubly-stochastic matrices.
 * https://oeis.org/A006847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + (n-1)^2*a(n-2) - ((n-1)*(n-2)/2)*a(n-3) -
 *            (n-1)*(n-2)*(n-3)*a(n-4) with a(0) = 1, a(1) = 1, a(2) = 2, a(3)
 *            = 5
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
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
    
    if (computed_up_to > n) {
        return cache[n];
    }
    
    if (cache == NULL) {
        cache_size = (n < 100) ? 100 : n + 10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 5;
        computed_up_to = 3;
    }
    
    while (n >= cache_size) {
        cache_size *= 2;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t m = (seq_t)(i - 1);
        seq_t m1 = (seq_t)(i - 2);
        seq_t m2 = (seq_t)(i - 3);
        
        cache[i] = cache[i-1] 
            + m * m * cache[i-2]
            - (m * m1 / 2) * cache[i-3]
            - m * m1 * m2 * cache[i-4];
    }
    
    computed_up_to = n;
    
    return cache[n];
}
