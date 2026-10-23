/*
 * A171479  a(n) = 6*a(n-1)-8*a(n-2)+3 for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A171479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) + 3 where a(0) = 1, a(1) = 8 (for n >
 *            1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed values */
    static seq_t *cache = NULL;          /* pointer to cached terms   */
    static size_t cap = 0;               /* current size of the cache */
    static idx_t maxidx = (idx_t)-1;     /* highest index computed   */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)calloc(2, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* OOM guard           */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cap    = 2;
        maxidx = (idx_t)1;
    }

    /* fast return for already computed indices */
    if (n <= maxidx) {
        return cache[(size_t)n];
    }

    /* grow cache if required */
    size_t need = (size_t)(n + 1);
    if (cap < need) {
        size_t new_cap = cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* OOM guard         */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = maxidx + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)6 * cache[(size_t)(i - 1)]
          - (seq_t)8 * cache[(size_t)(i - 2)]
          + (seq_t)3;
    }
    maxidx = n;

    return cache[(size_t)n];
}
