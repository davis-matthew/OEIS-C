/*
 * A122788  (1,3)-entry of the 3 X 3 matrix M^n, where M = {{0, -1, 1}, {1, 1,
 *          0}, {0, 1, 1}}.
 * https://oeis.org/A122788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) Initial values: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=0
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 55, u64 n <= 106,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised results. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;  /* number of stored entries */
    static idx_t  cache_cap = 0;  /* allocated capacity */

    /* Base initialization: ensure indices 0..3 are present. */
    if (cache == NULL) {
        idx_t cap = (n + 1 > 4) ? (n + 1) : 4;   /* at least 4 elements */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* safe fallback */

        cache_cap = cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache_len = 4;
    }

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary. */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* safe fallback */
        cache = tmp;
        cache_cap = needed;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i-1];
        val -= (seq_t)2 * cache[i-2];
        val += (seq_t)2 * cache[i-3];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
