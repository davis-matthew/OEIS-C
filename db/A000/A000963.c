/*
 * A000963  The convergent sequence B_n for the ternary continued fraction
 *          (3,1;2,2) of period 2.
 * https://oeis.org/A000963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 7*a(n-2) - 3*a(n-4) + a(n-6), n >= 6
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial: a(0)=0, a(1)=1, a(2)=0, a(3)=3, a(4)=7, a(5)=16 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed = 5;

seq_t a(idx_t n) {
    if (n < 6) {
        static const seq_t init[] = {0, 1, 0, 3, 7, 16};
        return init[n];
    }
    
    if (cache == NULL) {
        cache_size = 128;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = 3;
        cache[4] = 7;
        cache[5] = 16;
    }
    
    while (n >= cache_size) {
        cache_size *= 2;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }
    
    for (idx_t i = computed + 1; i <= n; i++) {
        cache[i] = 7 * cache[i - 2] - 3 * cache[i - 4] + cache[i - 6];
    }
    
    computed = n;
    return cache[n];
}
