/*
 * A228538  y-values in the solution to the Pell equation x^2 - 58*y^2 = -1.
 * https://oeis.org/A228538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=13, a(2)=509665, a(n) = 39206*a(n-1) - a(n-2)  (n>2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset is 1.  For n < 1 the behaviour is irrelevant. */
    if (n == 0)
        return (seq_t)0;            /* unused, but defined for safety */

    /* Convert to zero‑based index: cache[0] holds a(1). */
    idx_t idx = n - 1;

    /* Static cache: dynamically grow as needed. */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of terms stored (>=2 once init) */
    static idx_t  cache_cap = 0;   /* allocated capacity */

    /* Initialisation on the first call. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)13;         /* a(1) */
        cache[1] = (seq_t)509665;     /* a(2) */
        cache_len = 2;
    }

    /* Already cached?  If so, just return it. */
    if (idx < cache_len)
        return cache[idx];

    /* Grow the cache if necessary. */
    if (cache_cap <= idx) {
        while (cache_cap <= idx)
            cache_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute new terms up to index idx using the recurrence. */
    for (idx_t i = cache_len; i <= idx; ++i)
        cache[i] = (seq_t)39206 * cache[i - 1] - cache[i - 2];

    /* Update the number of terms stored. */
    cache_len = idx + 1;

    return cache[idx];
}
