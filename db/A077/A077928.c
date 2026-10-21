/*
 * A077928  Expansion of (1-x)^(-1)/(1+2*x+x^2-2*x^3).
 * https://oeis.org/A077928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1)+a(n-2)+3*a(n-3)-2*a(n-4) , a(0)=1, a(1)=-1,
 *            a(2)=2, a(3)=0
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of computed terms */
static size_t cache_capacity = 0;  /* allocated capacity */
static size_t cache_used = 0;      /* number of terms stored */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_capacity = cache_capacity ? cache_capacity * 2 : 16;
    while (new_capacity < needed)
        new_capacity *= 2;
    void *tmp = realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp)
        exit(1);                     /* allocation failed */
    cache = (seq_t *)tmp;
    cache_capacity = new_capacity;
}

/* Initialise the first four terms.  This is only done once. */
static void init_cache(void)
{
    if (cache == NULL)
        ensure_cache_size(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)0;
    cache_used = 4;
}

/* seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    /* Prepare the cache only once. */
    if (cache == NULL || cache_used < 4)
        init_cache();

    /* Convert index to size_t for array access.  On a 64‑bit build
       this is safe for the range of n used in the tests (up to ~10 000). */
    size_t idx = (size_t)n;

    /* Return a cached value if we already know it. */
    if (idx < cache_used)
        return cache[idx];

    /* Grow the cache to accommodate index n. */
    size_t needed = idx + 1;
    ensure_cache_size(needed);

    /* Compute any missing terms. */
    for (size_t i = cache_used; i < needed; ++i) {
        seq_t val =
            -cache[i - 1] + cache[i - 2] + (seq_t)3 * cache[i - 3] - (seq_t)2 * cache[i - 4];
        cache[i] = val;
    }
    cache_used = needed;

    return cache[idx];
}
