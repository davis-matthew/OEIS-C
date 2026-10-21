/*
 * A075886  Number of cubes at generation n when building fractal cube with
 *          edge ratio of 1/2.
 * https://oeis.org/A075886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2) - 24*a(n-3) for n >= 4 a(1) = 1, a(2)
 *            = 6, a(3) = 30 (Offset = 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* one‑indexed array of terms */
static idx_t cache_len = 0;   /* largest n that has been computed */

/* extend the cache up to index n (inclusive) and compute missing terms */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* allocate enough space for indices 0..n (index 0 unused) */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)   /* failed allocation – abort */
        abort();

    cache = new_cache;

    /* compute missing entries */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)6;
        else if (i == 3)
            cache[3] = (seq_t)30;
        else
            cache[i] = 5 * cache[i - 1] + 4 * cache[i - 2] - 24 * cache[i - 3];
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined offset – return 0 safely */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
