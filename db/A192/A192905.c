/*
 * A192905  Coefficient of x in the reduction by (x^2 -> x + 1) of the
 *          polynomial p(n,x) defined below at Comments.
 * https://oeis.org/A192905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + a(n-4) Initial values: a(0)=0,
 *            a(1)=1, a(2)=3, a(3)=8
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Allocate and seed the cache on first call. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 8;
        cache_size = 4;
    }

    /* Return pre‑computed value if available. */
    if (n < cache_size)
        return cache[(idx_t)n];

    /* Grow the cache to accommodate up to n. */
    if (cache_cap <= n) {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
    }

    /* Compute new entries using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1] + cache[i - 3] + cache[i - 4];
    }
    cache_size = n + 1;
    return cache[n];
}
