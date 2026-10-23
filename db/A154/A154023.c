/*
 * A154023  a(n+2) = 36*a(n+1) - a(n), a(1)=0, a(2)=6.
 * https://oeis.org/A154023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+2) = 36*a(n+1) - a(n), a(1)=0, a(2)=6 */
#include <stdlib.h>

/* Compute a(n) for OEIS A154023, with memoization.
   Valid for any build where seq_t and idx_t are defined. */
seq_t a(idx_t n)
{
    /* Static cache: store computed values from a(1) to a(cache_n). */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0; /* number of values stored (highest index) */

    if (n == 0)          /* safety: offset starts at 1 */
        return 0;

    /* initialize cache for first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        cache[0] = 0;        /* a(1) */
        cache[1] = 6;        /* a(2) */
        cache_n = 2;
    }

    /* if already computed up to n, just return */
    if (n <= cache_n)
        return cache[n - 1];

    /* extend the cache up to n */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL) { /* allocation failed, hang here */
        /* In the unlikely event of OOM, return 0 to avoid crash. */
        return 0;
    }
    cache = new_cache;

    /* Compute successive terms: recurrence rearranged to use previous two values */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 2];   /* a(i-1) */
        seq_t a_nm2 = cache[i - 3];   /* a(i-2) */
        cache[i - 1] = (a_nm1 * (seq_t)36) - a_nm2;
    }
    cache_n = n;

    return cache[n - 1];
}
