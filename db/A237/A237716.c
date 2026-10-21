/*
 * A237716  7-distance Pell sequence.
 * https://oeis.org/A237716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-7) + a(n-2) (n ≥ 7), a(0) = a(1) = a(2) = a(3) =
 *            a(4) = a(5) = a(6) = 1
 * Range:     u8 n <= 26, u16 n <= 49, u32 n <= 96, u64 n <= 189,
 *            u128 n <= 376, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required for memory allocation */
#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;       /* array of computed terms */
static size_t cache_size = 0;     /* number of elements currently initialized */
static size_t cache_cap = 0;      /* current capacity of the cache */

/* Compute the 7‑distance Pell sequence */
seq_t a(idx_t n)
{
    /* If we already have the value, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache has enough capacity */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 6)
            cache[i] = (seq_t)1;
        else
            cache[i] = cache[i - 2] + (cache[i - 7] * ((seq_t)2));
    }

    /* Update the count of initialized terms */
    cache_size = (size_t)n + 1;

    return cache[n];
}
