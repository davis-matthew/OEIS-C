/*
 * A226591  Expansion of g.f.
 *          (x*(1+x)*(1-x+x^2)*(1+x+x^2)*(1-x^2+x^4))/(1-x^2+x^4-x^5-x^6+x^7-x^9).
 * https://oeis.org/A226591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for 1 ≤ n ≤ 5 ; a(n) = a(n-3) + a(n-5) for 6 ≤ n ≤ 12 ;
 *            a(n) = a(n-5) + a(n-8) + a(n-11) for n ≥ 13
 * Range:     u8 n <= 40, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of already computed terms (1‑based index stored at index-1). */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cached_to = 0;   /* highest n for which cache[n-1] is valid */

/* Increase capacity of the cache to at least n elements. */
static void ensure_cache(idx_t n) {
    if (cache_cap >= n)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);            /* fatal error: unable to allocate memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n) {
    /* Assume n ≥ 1. */
    if (n < 1)
        return 0;          /* undefined for 0, but return 0 */

    /* Grow cache if necessary. */
    if (cache_cap < n)
        ensure_cache(n);

    /* Compute any missing terms up to n. */
    if (cached_to < n) {
        for (idx_t i = cached_to + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i - 1] = (seq_t)1;
            } else if (i <= 12) {
                cache[i - 1] = cache[i - 3 - 1] + cache[i - 5 - 1];
            } else {
                cache[i - 1] = cache[i - 5 - 1] +
                               cache[i - 8 - 1] +
                               cache[i - 11 - 1];
            }
        }
        cached_to = n;
    }

    return cache[n - 1];
}
