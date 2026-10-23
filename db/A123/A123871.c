/*
 * A123871  Expansion of g.f.: (1+x+x^2)/(1-4*x-4*x^2).
 * https://oeis.org/A123871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) for n>2; initial values: a(0)=1,
 *            a(1)=5, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoization buffer */
    static size_t cap = 0;               /* current capacity */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* error path, unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)25;
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap *= 2;

        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* error path */

        cache = tmp;
        /* Compute new entries using the recurrence */
        for (size_t i = cap; i < newcap; ++i) {
            if (i < 3) continue;         /* already initialized */
            cache[i] = 4 * cache[i-1] + 4 * cache[i-2];
        }
        cap = newcap;
    }

    return cache[(size_t)n];
}
