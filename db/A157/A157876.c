/*
 * A157876  Let f(x) = 1 + x^2 + x^4 + x^5 + x^6 + x^10 + x^11; sequence has
 *          g.f. g(x) = 1/(x^11*f(1/x)).
 * https://oeis.org/A157876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(a(n‑1) + a(n‑5) + a(n‑6) + a(n‑7) + a(n‑9) + a(n‑11)),
 *            a(0) = 1 (indices < 0 are treated as 0)
 * Range:     i8 n <= 18, i16 n <= 36, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;        /* cached values of the sequence   */
static size_t cache_cap = 0;       /* allocated capacity of cache     */
static size_t cache_len = 0;       /* next index to compute           */

/* Allocate a cache of the given capacity and zero‑initialize it. */
static void init_cache(size_t cap)
{
#ifdef __cplusplus
    cache = new seq_t[cap];
    for (size_t i = 0; i < cap; ++i)
        cache[i] = 0;
#else
    cache = malloc(cap * sizeof(seq_t));
    if (cache)
        memset(cache, 0, cap * sizeof(seq_t));
#endif
    if (cache)
        cache_cap = cap;
}

/* Ensure that the cache can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL)
        init_cache((size_t)n + 1);
}

/* Grow the cache to at least n+1 entries. */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed;
    if (new_cap < 1024)
        new_cap = 1024;
    else
        new_cap <<= 1;

    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < new_cap; ++i)
        new_cache[i] = 0;
#else
    new_cache = malloc(new_cap * sizeof(seq_t));
    if (new_cache)
        memset(new_cache, 0, new_cap * sizeof(seq_t));
#endif
    if (!new_cache)
        return;   /* allocation failed – leave cache unchanged */

    /* Copy old data */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* Free old buffer */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Required public function:   seq_t a(idx_t n)                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Ensure sufficient cache space */
    if (cache == NULL)
        ensure_cache(n);
    if (cache == NULL)  /* allocation failed */
        return (seq_t)0;

    if (cache_cap <= (size_t)n + 1)
        grow_cache(n);
    if (cache == NULL) /* allocation failed after grow */
        return (seq_t)0;

    /* Initialize first element if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        if (i >= 1)  sum += cache[i - 1];
        if (i >= 5)  sum += cache[i - 5];
        if (i >= 6)  sum += cache[i - 6];
        if (i >= 7)  sum += cache[i - 7];
        if (i >= 9)  sum += cache[i - 9];
        if (i >= 11) sum += cache[i - 11];
        cache[i] = -sum;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
