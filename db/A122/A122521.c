/*
 * A122521  Recursion: a(n) = a(n - 6) + a(n - 8).
 * https://oeis.org/A122521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 116, u32 n <= 226, u64 n <= 450,
 *            u128 n <= 894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-6) + a(n-8), a(1..8) = 1 */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* 1‑based indexing for a(n) */
static size_t cache_cap = 0;         /* current allocated size (excluding index 0) */
static size_t cache_len = 0;         /* highest index for which a(n) is computed */

/* Ensure the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;                      /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)n)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* allocation failure */

    if (cache)
    {
        memcpy(new_cache + 1, cache + 1, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise the first 8 terms to 1 */
    if (cache_len < 8)
    {
        for (size_t i = 1; i <= 8; ++i)
            cache[i] = (seq_t)1;
        cache_len = 8;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour below the offset does not matter */
        return (seq_t)0;

    if (n <= 8)
        return (seq_t)1;

    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i)
    {
        cache[i] = cache[i - 6] + cache[i - 8];
    }
    cache_len = (size_t)n;

    return cache[n];
}
