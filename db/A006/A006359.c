/*
 * A006359  Number of antichains in the product of a fence poset with n
 *          elements and a chain with 5 elements; also number of paths with n
 *          turns when light is reflected from 6 glass plates.
 * https://oeis.org/A006359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 4*a(n-3) - 5*a(n-4) + a(n-5) +
 *            a(n-6), a(0) = 1, a(1) = 6, a(2) = 21, a(3) = 91, a(4) = 371,
 *            a(5) = 1547
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_size = (n < 6) ? 6 : (n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)91;
        cache[4] = (seq_t)371;
        cache[5] = (seq_t)1547;
        
        for (idx_t i = 6; i < cache_size; i++) {
            cache[i] = 3*cache[i-1] + 6*cache[i-2] - 4*cache[i-3] - 5*cache[i-4] + cache[i-5] + cache[i-6];
        }
    } else if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        
        for (idx_t i = old_size; i < cache_size; i++) {
            cache[i] = 3*cache[i-1] + 6*cache[i-2] - 4*cache[i-3] - 5*cache[i-4] + cache[i-5] + cache[i-6];
        }
    }
    
    return cache[n];
}
