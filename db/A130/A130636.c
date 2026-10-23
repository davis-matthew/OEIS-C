/*
 * A130636  a(n) = n*a(n-3) + a(n-4) for n >= 4.
 * https://oeis.org/A130636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n*a(n-3)+a(n-4) for n >= 4
   Initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* actual cached size */

    /* return the small indexed terms immediately */
    if (n < 4) {
        switch ((idx_t)n) {
            case 0ULL: return (seq_t)0;
            case 1ULL: return (seq_t)1;
            case 2ULL: return (seq_t)0;
            case 3ULL: return (seq_t)0;
        }
    }

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
    }

    /* extend cache to include n if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* reallocation failed */
        cache = tmp;

        /* compute new values iteratively */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)i * cache[i - 3] + cache[i - 4];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
