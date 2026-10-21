/*
 * A224130  Number of n X 5 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3) + 2*a(n-4) + 8*a(n-5)
 *            (offset 1, a(1)=6, a(2)=36, a(3)=175, a(4)=760, a(5)=3149)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    /* static cache for memoisation */
    static seq_t *cache = (seq_t*)0;
    static idx_t cached_max = 0;

    /* initialise first five values if not yet done */
    if (cached_max < 5) {
#ifdef __cplusplus
        cache = new seq_t[5];
#else
        cache = (seq_t*)malloc(5 * sizeof(seq_t));
#endif
        if (!cache) return 0;
        cache[0] = 6;
        cache[1] = 36;
        cache[2] = 175;
        cache[3] = 760;
        cache[4] = 3149;
        cached_max = 5;
    }

    /* extend cache if needed */
    if (n > cached_max) {
        idx_t new_max = n;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_max];
        for (idx_t i = 0; i < cached_max; ++i) new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_max * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failed */
        cache = new_cache;
#endif
        for (idx_t i = cached_max; i < new_max; ++i) {
            cache[i] = cache[i-1] * 6
                     - cache[i-2] * 9
                     + cache[i-3] * 4
                     + cache[i-4] * 2
                     + cache[i-5] * 8;
        }
        cached_max = new_max;
    }

    return cache[n-1];
}
