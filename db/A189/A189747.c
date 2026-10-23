/*
 * A189747  a(1)=5, a(2)=3, a(n)=5*a(n-1) + 3*a(n-2).
 * https://oeis.org/A189747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 5, a(2) = 3, a(n) = 5*a(n-1) + 3*a(n-2) for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* index zero is unused; a(1) is the first term */
    if (n < 1)
        return (seq_t)0;        /* behaviour for n < 1 is irrelevant */

    static seq_t *cache = NULL;    /* cache[1 .. cache_max] holds computed terms */
    static idx_t  cache_max = 0;   /* highest index currently stored */

    /* if we already have the requested element, return it */
    if (n <= cache_max)
        return cache[n];

    /* remember the previous maximum so we know where to start the
       recurrence after reallocation */
    idx_t old_max = cache_max;

    /* enlarge the cache to accommodate index n */
    size_t new_size = (size_t)n + 1;               /* indices 0 .. n (0 unused) */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        abort();                                 /* allocation failed */

    /* copy any previously cached values */
    for (idx_t i = 1; i <= old_max; ++i)
        new_cache[i] = cache[i];

    /* free the old cache (may be NULL) and use the new one */
    free(cache);
    cache = new_cache;

    /* set the base values (overwriting if they already exist) */
    if (n >= 1)
        cache[1] = (seq_t)5;
    if (n >= 2)
        cache[2] = (seq_t)3;

    /* compute missing terms using the recurrence.
       If we had computed up to old_max ≥ 2, start at old_max+1;
       otherwise (old_max == 0 or 1) start at 3. */
    idx_t start = (old_max >= 2) ? old_max + 1 : 3;
    for (idx_t i = start; i <= n; ++i)
    {
        cache[i] = (seq_t)5 * cache[i-1] + (seq_t)3 * cache[i-2];
    }

    cache_max = n;        /* update the maximum index stored */
    return cache[n];
}
