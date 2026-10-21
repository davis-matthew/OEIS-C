/*
 * A065597  a(0)=0, a(1)=1, a(2)=1; for n >= 3, a(n) = 2*a(n-1)*a(n-2) -
 *          a(n-3).
 * https://oeis.org/A065597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=1; a(2)=1; for n≥3, a(n)=2*a(n‑1)*a(n‑2)-a(n‑3)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache keeps already computed terms; grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_used = 0;  /* number of terms currently stored */

    /* Initialise the first three values on first call */
    if (cache == NULL) {
        size_t init_cap = 3;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_used = 3;
    }

    /* If the requested index is already cached, return it */
    size_t idx = (size_t)n;
    if (idx < cache_used) {
        return cache[idx];
    }

    /* Grow the cache if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : idx + 1;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from the first missing index up to n */
    for (size_t i = cache_used; i <= idx; ++i) {
        seq_t prod = cache[i-1] * cache[i-2];
        cache[i] = prod + prod - cache[i-3];
    }
    cache_used = idx + 1;

    return cache[idx];
}
