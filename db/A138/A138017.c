/*
 * A138017  Expansion of g.f. x*(1-6*x^2)/(1-3*x-2*x^2+6*x^3).
 * https://oeis.org/A138017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 6*a(n-3), with a(1) = 1, a(2) = 3,
 *            a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based array of computed terms   */
static size_t cache_cap = 0;     /* current allocated size (>=4)      */
static idx_t  cache_max = 0;     /* highest n that has been computed */

static void init_cache(void)
{
    if (!cache) {
        cache = (seq_t*)calloc(4, sizeof(seq_t));
        cache_cap = 4;
        cache[0] = (seq_t)0;      /* unused, keeps 1‑based indexing   */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache_max = 3;
    }
}

/* Grow the cache so that indices up to n will be available. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    init_cache();

    /* grow the allocation if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);    /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
        /* initialise any newly allocated memory to zero */
        for (size_t i = cache_max + 1; i <= n; ++i) cache[i] = 0;
    }

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)    cache[i] = (seq_t)1;
        else if (i == 2) cache[i] = (seq_t)3;
        else if (i == 3) cache[i] = (seq_t)5;
        else
            cache[i] = (seq_t)3 * cache[i-1] + (seq_t)2 * cache[i-2]
                       - (seq_t)6 * cache[i-3];
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* out of range, no defined value */
    ensure_cache(n);
    return cache[(size_t)n];
}
