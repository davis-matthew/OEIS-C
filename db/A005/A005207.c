/*
 * A005207  a(n) = (F(2*n-1) + F(n+1))/2 where F(n) is a Fibonacci number.
 * https://oeis.org/A005207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(2*n-1) + F(n+1))/2 Implemented using recurrence: a(n)
 *            = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;
    
    if (cache == NULL) {
        cache_size = 2000;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 4;
        max_computed = 3;
    }
    
    if (n <= max_computed) {
        return cache[n];
    }
    
    while (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 4 * cache[i-1] - 3 * cache[i-2] - 2 * cache[i-3] + cache[i-4];
    }
    max_computed = n;
    
    return cache[n];
}
