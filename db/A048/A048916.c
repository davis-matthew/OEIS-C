/*
 * A048916  Indices of 9-gonal numbers which are also hexagonal.
 * https://oeis.org/A048916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64514*a(n-2) - a(n-4) - 23040 (for n >= 5) Initial
 *            values: a(1)=1, a(2)=10, a(3)=39025, a(4)=621946
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache indexed by n (1‑based), dynamic expansion on demand */
    static seq_t *cache      = NULL;   /* cache[0] unused */
    static idx_t cache_len   = 0;       /* highest n that is cached */

    /* initialise cache with the first four terms (only if first call) */
    if (!cache)
    {
        /* allocate entries 0..4 (size 5) */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) { abort(); }
        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)39025;
        cache[4] = (seq_t)621946;
        cache_len = 4;
    }

    /* if the requested term is already stored, just return it */
    if (n <= cache_len) return cache[n];

    /* expand cache to at least n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) { abort(); }
    cache = new_cache;

    /* compute missing terms using the linear recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* a(i) = 64514*a(i-2) - a(i-4) - 23040  (for i>=5) */
        /* for i==5, i-4 >=1 so recurrence is valid */
        cache[i] = (seq_t)64514 * cache[i - 2] - cache[i - 4] - (seq_t)23040;
    }

    cache_len = n;
    return cache[n];
}
