/*
 * A010035  a(n) = 2*3^(2*n)-3^n.
 * https://oeis.org/A010035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 27*a(n-2), a(0)=1, a(1)=15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    size_t needed = (size_t)n;

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)15;  /* a(1) */
        cache_len = 2;
    }

    /* extend cache if necessary */
    if (needed >= cache_len) {
        /* ensure capacity for indices up to 'needed' */
        if (needed + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap <= needed) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= needed; ++i) {
            seq_t term1 = cache[i - 1] * ((seq_t)12);
            seq_t term2 = cache[i - 2] * ((seq_t)27);
            cache[i] = term1 - term2;
        }
        cache_len = needed + 1;
    }

    return cache[needed];
}
