/*
 * A085349  Ratio-determined insertion sequence I(0.26688) (see the link
 *          below).
 * https://oeis.org/A085349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=4, a(3)=15, a(4)=71, a(n) = 18*a(n-2) - a(n-4) for
 *            n >= 5 (OEIS A085349)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation across calls (any order) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms already computed (>=0) */

    if (n < 1)  /* behaviour for indices below the offset is unspecified */
        return (seq_t)0;

    /* Initialise the cache with the first four terms if not yet done */
    if (cached == 0) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));   /* indices 1..4 used */
        if (!cache) return (seq_t)0;                 /* allocation failed */
        cache[0] = (seq_t)0;                         /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)71;
        cached = 4;
    }

    /* If the requested index is already cached, return it immediately */
    if (n <= cached)
        return cache[n];

    /* Grow the cache to accommodate n (index 1..n) */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failed */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* For i >= 5 the recurrence is valid:
           a(i) = 18 * a(i-2) - a(i-4) */
        cache[i] = ((seq_t)18) * cache[i - 2] - cache[i - 4];
    }

    cached = n;
    return cache[n];
}
