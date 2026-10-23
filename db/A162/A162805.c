/*
 * A162805  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 
 * Sequence A162805: Number of reduced words of length n in the Coxeter
 * group on 19 generators.
 *
 * Recurrence:
 *   a(0) = 1
 *   a(1) = 19
 *   a(2) = 342
 *   a(3) = 5985
 *   for n >= 4:
 *       a(n) = 17*a(n-1) + 17*a(n-2) - 153*a(n-3)
 */
seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of elements currently stored */

    /* Allocate initial cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Grow the cache if we need to store a larger index */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute all missing values up to n (inclusive) */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)19;
        } else if (i == 2) {
            cache[2] = (seq_t)342;
        } else if (i == 3) {
            cache[3] = (seq_t)5985;
        } else {
            cache[i] = ( (seq_t)17 * cache[i-1] )
                       + ( (seq_t)17 * cache[i-2] )
                       - ( (seq_t)153 * cache[i-3] );
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
