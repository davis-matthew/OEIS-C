/*
 * A026390  Expansion of (2 + x + x^2)/((1 - x)*(1 - x - x^2)).
 * https://oeis.org/A026390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n >= 3, with a(0) = 2, a(1) = 5,
 *            a(2) = 11
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t max_computed = (idx_t)-1;

/* Ensure the cache can hold values up to n, allocating or expanding if necessary.
   Also sets the base values a(0)..a(2). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        size_t cap = n + 1;
        if (cap < 3) cap = 3;                /* space for base terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return;                 /* allocation failure */
        cache[0] = 2; cache[1] = 5; cache[2] = 11;
        cache_cap = cap;
        max_computed = 2;
    } else if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
        if (max_computed < 2) {
            cache[0] = 2; cache[1] = 5; cache[2] = 11;
            max_computed = 2;
        }
    }
}

/* Computes a(n) for any n >= 0, caching intermediate results. */
seq_t a(idx_t n)
{
    /* Small indices are handled directly */
    if (n == 0) return 2;
    if (n == 1) return 5;
    if (n == 2) return 11;

    ensure_cache(n);
    if (max_computed >= n) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1] - cache[i - 3];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
