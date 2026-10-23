/*
 * A042151  Denominators of continued fraction convergents to sqrt(600).
 * https://oeis.org/A042151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 98*a(n-2) - a(n-4) for n > 3 a(0) = 1, a(1) = 2, a(2) =
 *            97, a(3) = 196
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;    /* number of filled entries (first index is 0) */
static size_t cache_cap = 0;    /* allocated capacity (in elements) */

/* Ensure the cache can hold indices up to n (inclusive) */
static void grow_cache(idx_t n)
{
    /* Compute the required number of elements (n+1 because 0‑based) */
    size_t needed = (size_t)n + 1;

    if (needed <= cache_cap)
        return;                      /* already big enough */

    size_t new_cap = cache_cap > 0 ? cache_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;                /* double until we have enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                          /* allocation failure: terminate */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A042151. */
seq_t a(idx_t n)
{
    /* Small indices handled explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)97;
    if (n == 3) return (seq_t)196;

    /* Ensure we can store up to index n. */
    grow_cache(n);

    /* Initialise the first four cached values if not already present. */
    if (cache_len < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)97;
        cache[3] = (seq_t)196;
        cache_len = 4;
    }

    /* Compute any missing entries up to n. */
    if ((idx_t)cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i ≥ 4 guarantees that i-2 and i-4 are valid indices. */
            cache[i] = (seq_t)98 * cache[i - 2] - cache[i - 4];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
