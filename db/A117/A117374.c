/*
 * A117374  Expansion of (1-3x)/(1-x^2+x^3).
 * https://oeis.org/A117374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 35, i32 n <= 75, i64 n <= 154,
 *            i128 n <= 312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117374:  a(n) = a(n-2) - a(n-3);
   Initial values: a(0)=1, a(1)=-3, a(2)=1. */

#include <stddef.h>
#include <stdlib.h>

/* Cache for already computed terms */
static seq_t *cache      = NULL;  /* array of seq_t */
static size_t cache_len  = 0;     /* number of entries filled */
static size_t cache_cap  = 0;     /* allocated size */

/* Ensure the cache is large enough for indices 0..n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory – stop */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Initialise the first three values if needed */
static void init_cache(void)
{
    if (cache_len < 3) {
        if (cache_cap < 3) {
            free(cache);
            cache = (seq_t *)malloc(3 * sizeof(seq_t));
            if (!cache) abort();
            cache_cap = 3;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)1;
        cache_len = 3;
    }
}

/* Main sequence function – accepts calls in any order */
seq_t a(idx_t n)
{
    ensure_cache(n);
    init_cache();

    /* Return already cached value if available */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[(size_t)i] = cache[(size_t)(i - 2)] - cache[(size_t)(i - 3)];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
