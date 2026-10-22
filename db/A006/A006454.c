/*
 * A006454  Solution to a Diophantine equation: each term is a triangular
 *          number and each term + 1 is a square.
 * https://oeis.org/A006454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*(a(n-2) - a(n-4)) + a(n-6) Initial values: a(0) = 0,
 *            a(1) = 3, a(2) = 15, a(3) = 120, a(4) = 528, a(5) = 4095
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 0;
    
    if (cache == NULL) {
        cache_len = 6;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 3;
        cache[2] = 15;
        cache[3] = 120;
        cache[4] = 528;
        cache[5] = 4095;
        computed_up_to = 5;
    }
    
    if (cache_len <= n) {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }
    
    while (computed_up_to < n) {
        computed_up_to++;
        cache[computed_up_to] = 35 * (cache[computed_up_to - 2] - cache[computed_up_to - 4]) 
                                 + cache[computed_up_to - 6];
    }
    
    return cache[n];
}
