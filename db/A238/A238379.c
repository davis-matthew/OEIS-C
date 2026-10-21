/*
 * A238379  Expansion of (1 - x)/(1 - 36*x + x^2).
 * https://oeis.org/A238379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Recurrence: a(0)=1, a(1)=35, a(n)=36*a(n-1)-a(n-2) (n>=2) */
static seq_t *cache     = NULL;   /* stores a(0) … a(k) */
static size_t cache_cap = 0;      /* number of allocated elements  */
static size_t cache_size = 0;     /* number of initialized elements */

/* Compute a(n) using memoized recurrence */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly – avoids any array usage. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)35;

    /* Ensure capacity for at least the first two elements.           */
    if (cache_cap < 2) {
        size_t new_cap = 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* unrecoverable */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* When the cache is empty, initialise a(0) and a(1). */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)35;
        cache_size = 2;
    }
    /* If the cache has only one element (possible after a failed call)   */
    if (cache_size == 1) {
        cache[1] = (seq_t)35;
        cache_size = 2;
    }

    /* Grow the cache to accommodate all terms up to n. */
    size_t needed = (size_t)n + 1;            /* number of elements required */
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* unrecoverable */
        cache     = tmp;
        cache_cap = needed;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)36 - cache[i - 2];
    }
    cache_size = needed;   /* all terms up to n are now initialised */

    return cache[n];
}
