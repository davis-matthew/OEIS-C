/*
 * A001925  From rook polynomials.
 * https://oeis.org/A001925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 2*a(n-3) + 6*a(n-4) - 4*a(n-5) -
 *            a(n-6) + a(n-7), a(0)=1, a(1)=6, a(2)=22, a(3)=64, a(4)=162,
 *            a(5)=374, a(6)=809
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 33, u64 n <= 77, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    static const seq_t initial[] = {1, 6, 22, 64, 162, 374, 809};
    
    if (n < 7) {
        return initial[n];
    }
    
    if (cache == NULL) {
        cache_size = 128;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (int i = 0; i < 7; i++) {
            cache[i] = initial[i];
        }
        computed_up_to = 6;
    }
    
    while (n >= cache_size) {
        cache_size *= 2;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t val = (seq_t)5 * cache[i-1]
                  - (seq_t)8 * cache[i-2]
                  + (seq_t)2 * cache[i-3]
                  + (seq_t)6 * cache[i-4]
                  - (seq_t)4 * cache[i-5]
                  - cache[i-6]
                  + cache[i-7];
        cache[i] = val;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return cache[n];
}
