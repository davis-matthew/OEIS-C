/*
 * A095934  Expansion of (1-x)^2/(1-5*x+3*x^2).
 * https://oeis.org/A095934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 5*a(n+1) - 3*a(n), a(0)=1, a(1)=3, a(2)=13
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* stored terms */
static idx_t cache_size = 0;     /* next free index (== current size) */

seq_t a(idx_t n)
{
    /* handle the very first terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;

    /* initialise or extend the cache to at least n+1 entries */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)13;
        cache_size = 3;
    }

    if (n < cache_size)
        return cache[n];

    /* grow the array if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* old size is cache_size */
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)5 * cache[i - 1];
        val -= (seq_t)3 * cache[i - 2];
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
