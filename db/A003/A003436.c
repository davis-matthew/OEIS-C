/*
 * A003436  Number of inequivalent labeled Hamiltonian circuits on
 *          n-octahedron. Interlacing chords joining 2n points on circle.
 * https://oeis.org/A003436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n*a(n-1) - 2*(n-3)*a(n-2) - a(n-3) for n > 4 Base: a(0)
 *            = 1, a(1) = 0, a(2) = 1, a(3) = 4, a(4) = 31
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
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
    
    if (n < cache_len) {
        return cache[n];
    }
    
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 4;
        cache[4] = 31;
        cache_len = 5;
    }
    
    while (cache_cap <= n) {
        idx_t new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    while (cache_len <= n) {
        idx_t i = cache_len;
        seq_t ni = (seq_t)i;
        seq_t ni_3 = (seq_t)(i - 3);
        seq_t term1 = (seq_t)2 * ni * cache[i - 1];
        seq_t term2 = (seq_t)2 * ni_3 * cache[i - 2];
        cache[i] = term1 - term2 - cache[i - 3];
        cache_len++;
    }
    
    return cache[n];
}
