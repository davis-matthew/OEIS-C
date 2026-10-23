/*
 * A187384  Number of 10-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for 1 ≤ n ≤ 3 a(4) = 280, a(5) = 3792, a(6) = 17052,
 *            a(7) = 44464, a(8) = 92628, a(9) = 159328, a(10) = 245946, a(11)
 *            = 350386 For n > 11: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3)
 *            (Derived from the OEIS entry A187384)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms currently stored  */

    /* Initialize cache at first call with the first 11 terms */
    if (cached == 0) {
        /* Allocate room for 11 terms initially */
        idx_t cap = 12;   /* a small excess to allow a quick grow later */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */

        cache[0] = 0;        /* a(1) */
        cache[1] = 0;        /* a(2) */
        cache[2] = 0;        /* a(3) */
        cache[3] = 280;      /* a(4) */
        cache[4] = 3792;     /* a(5) */
        cache[5] = 17052;    /* a(6) */
        cache[6] = 44464;    /* a(7) */
        cache[7] = 92628;    /* a(8) */
        cache[8] = 159328;   /* a(9) */
        cache[9] = 245946;   /* a(10) */
        cache[10] = 350386;  /* a(11) */

        cached = 11;         /* we have computed up to a(11) */
    }

    /* If the requested n is already cached, return it */
    if (n <= cached)
        return cache[n-1];

    /* Grow the cache to at least size n */
    if (cache) {
        idx_t needed = n;
        if (needed > cached*2) needed = cached*2;
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp) cache = tmp;  /* ignore reallocation failure; cache stays usable */
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached; i < n; ++i) {
        cache[i] = 3 * cache[i-1] - 3 * cache[i-2] + cache[i-3];
    }
    cached = n;   /* update cached size */

    return cache[n-1];
}
