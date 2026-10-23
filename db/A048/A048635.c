/*
 * A048635  Number of rational points of Klein curve over GF(2^n).
 * https://oeis.org/A048635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(n)=4*a(n-1)-7*a(n-2)+8*a(n-3)-4*a(n-4) */
/* Offset:1  (a(1)=0, a(2)=14, a(3)=24, a(4)=14) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size (highest index)      */
    static idx_t size  = 0;   /* highest index computed in cache     */

    /* initialise the cache with the first four terms */
    if (cap == 0) {
        cap   = 4;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failure */
        for (idx_t i = 0; i <= cap; ++i) cache[i] = 0;
        cache[1] = 0;
        cache[2] = 14;
        cache[3] = 24;
        cache[4] = 14;
        size = 4;
    }

    /* already known? */
    if (n <= size) return cache[n];

    /* grow the cache if necessary */
    if (n > cap) {
        idx_t new_cap = n + 1;                  /* at least up to n */
        if (new_cap < 8) new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * 4 - cache[i - 2] * 7
                    + cache[i - 3] * 8 - cache[i - 4] * 4;
        cache[i] = val;
    }
    size = n;
    return cache[n];
}
