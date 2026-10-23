/*
 * A160269  Numerator of Hermite(n, 15/29).
 * https://oeis.org/A160269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 15/29) Recurrence: a(0)=1, a(1)=30,
 *            a(n) = 30*a(n-1) - 1682*(n-1)*a(n-2) (n≥2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard headers for memory allocation */
#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache          = NULL;  /* array of computed terms */
static idx_t cache_alloc     = 0;     /* allocated capacity (≥ cache_len) */
static idx_t cache_len       = 0;     /* number of terms already computed */

/* Ensure that the cache has enough capacity for indices 0..need-1 */
static void ensure_cache(idx_t need)
{
    if (need <= cache_alloc)
        return;
    seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
    if (!new_cache)
        abort();
    /* copy existing entries */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache           = new_cache;
    cache_alloc     = need;
}

/* Return the nth term of the sequence.  Results are cached for reuse. */
seq_t a(idx_t n)
{
    /* Make sure the cache can store the requested index */
    ensure_cache(n + 1);

    /* initialise the base cases if they have not been stored yet */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        if (n == 0)
            return cache[0];
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)30;
        cache_len = 2;
        if (n == 1)
            return cache[1];
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prod = (seq_t)1682 * (seq_t)(i - 1) * cache[i - 2];
        seq_t term = (seq_t)30 * cache[i - 1];
        cache[i] = term - prod;
    }

    cache_len = n + 1;   /* update number of computed terms */
    return cache[n];
}
