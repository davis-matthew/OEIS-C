/*
 * A042415  Denominators of continued fraction convergents to sqrt(735).
 * https://oeis.org/A042415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) follows the linear recurrence * a(0)=1, a(1)=9, a(2)=487,
 *            a(3)=4392 * a(n) = 488*a(n-2) - a(n-4) for n >= 4 * (derived
 *            from the generating function -(x^2-9*x-1)/(x^4-488*x^2+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order and repeated calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current number of computed terms */
    static idx_t cache_cap = 0;   /* current allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* initial capacity larger than the 4 base terms */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* guard against allocation failure */

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)487;
        cache[3] = (seq_t)4392;
        cache_len = 4;
    }

    /* return early if we already have the term */
    if (n < cache_len)
        return cache[n];

    /* grow the cache until we have a(n) */
    while (cache_len <= n) {
        idx_t i = cache_len;      /* index of the new term */

        /* recurrence: a(i) = 488*a(i-2) - a(i-4) */
        seq_t new_term = (seq_t)488 * cache[i - 2] - cache[i - 4];

        /* ensure there is room for the new term */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap ? (size_t)cache_cap * 2 : 8);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;       /* guard against reallocation failure */
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        cache[cache_len] = new_term;
        cache_len++;
    }

    return cache[n];
}
