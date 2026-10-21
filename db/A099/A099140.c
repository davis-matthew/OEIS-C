/*
 * A099140  a(n) = 4^n * T(n,3/2) where T is the Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A099140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-1) - 16*a(n-2); a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache to store computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap = 0;

/* Ensure that the cache has space for at least n+1 terms and
   that the first two terms are initialized. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;                    /* allocation failed – leave empty */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache_size = 2;
    }

    if (n < cache_size) return;   /* already computed */

    /* Grow capacity if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return;           /* reallocation failed – keep old */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t p1 = cache[i - 1];
        seq_t p2 = cache[i - 2];
        cache[i] = (seq_t)(12 * p1 - 16 * p2);
    }
    cache_size = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    if (cache == NULL) return (seq_t)0; /* fallback if allocation failed */
    return cache[n];
}
