/*
 * A002467  The game of Mousetrap with n cards (given n letters and n
 *          envelopes, how many ways are there to fill the envelopes so that
 *          at least one letter goes into its right envelope?).
 * https://oeis.org/A002467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*a(n-1) - (-1)^n, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_cap = 1000;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache_len = 1;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        
        idx_t i = cache_len;
        seq_t prev = cache[i - 1];
        if (i % 2 == 0) {
            cache[i] = (seq_t)i * prev - (seq_t)1;
        } else {
            cache[i] = (seq_t)i * prev + (seq_t)1;
        }
        cache_len++;
    }
    
    return cache[n];
}
