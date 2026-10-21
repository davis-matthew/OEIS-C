/*
 * A052534  Expansion of (1-x)*(1+x)/(1-2*x-x^2+x^3).
 * https://oeis.org/A052534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(n)=2*a(n-1)+a(n-2)-a(n-3) for n>=3
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached dynamic evaluation of the recurrence for all n ≥ 0. */
    static seq_t *cache = NULL;   /* pointers to cached terms */
    static size_t cache_len = 0;  /* number of terms currently allocated */

    /* ensure base terms are present */
    if (cache_len == 0) {
        size_t init_len = 3;
        cache = (seq_t *)calloc(init_len, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache_len = init_len;
    }

    size_t idx = (size_t)n;

    if (idx >= cache_len) {
        /* extend cache to hold term idx */
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* unlikely */
        cache = tmp;

        /* compute new terms using the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* i >= 3 because cache_len is at least 3 */
            cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] - cache[i-3];
        }
        cache_len = new_len;
    }

    return cache[idx];
}
