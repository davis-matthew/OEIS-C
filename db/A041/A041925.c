/*
 * A041925  Denominators of continued fraction convergents to sqrt(485).
 * https://oeis.org/A041925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=44, a(n)=44*a(n-1)+a(n-2) for n>1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for computed values.  NULL means nothing allocated yet. */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;   /* allocated size of the array */
static idx_t computed_up_to = 0;    /* highest index whose value is known */

/* Ensure that the cache can hold indices 0 .. need. */
static void ensure_capacity(size_t need)
{
    if (need < cache_capacity)
        return;

    size_t new_cap = cache_capacity == 0 ? 2 : cache_capacity;
    while (new_cap <= need)
        new_cap <<= 1;   /* double until we have enough space */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)            /* allocation failed – nothing to do, cache stays as is */
        return;

    cache = tmp;
    cache_capacity = new_cap;
}

/* Public function required by the task. */
seq_t a(idx_t n)
{
    /* Fast path for the first two terms. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)44;

    /* Lazy allocation of the cache on the first non‑trivial call. */
    if (cache == NULL) {
        cache_capacity = 2;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache)          /* allocation failed – return 0 as a sentinel */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)44;
        computed_up_to = 1;
    }

    /* Compute any missing terms up to and including n. */
    if ((idx_t)computed_up_to < n) {
        /* Make sure the array has space for index n. */
        ensure_capacity((size_t)n);
        if (!cache)            /* allocation failed – return 0 */
            return (seq_t)0;

        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[(size_t)i] =
                (seq_t)44 * cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
        }
        computed_up_to = n;
    }

    return cache[(size_t)n];
}
