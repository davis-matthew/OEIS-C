/*
 * A086828  a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 7; thereafter, a(n) = a(n-1)
 *          + (n-1)*a(n-2).
 * https://oeis.org/A086828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=1, a(4)=7; for n≥5,
 *            a(n)=a(n‑1)+(n‑1)*a(n‑2)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small indices handled directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)7;

    static seq_t *cache   = NULL;   /* cache[i] holds a(i) for i ≥ 1          */
    static idx_t cache_len = 0;     /* currently allocated size (index of max + 1) */

    /* Grow the cache if necessary so that it contains a(1)…a(n) */
    if (cache_len <= n) {
        idx_t new_len = n + 1;                    /* slots 0…n   (0 unused) */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!new_cache) return 0;                 /* allocation failed – return 0 as a safe fallback */

        /* If this is the first allocation, initialise the base cases */
        if (!cache) {
            cache_len = new_len;
            cache = new_cache;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)1;
            cache[4] = (seq_t)7;
            /* compute entries 5…n using the recurrence */
            for (idx_t i = 5; i <= n; ++i)
                cache[i] = cache[i-1] + (seq_t)(i-1) * cache[i-2];
            return cache[n];
        }
        /* Cache existed; we need to compute only the new entries */
        for (idx_t i = cache_len; i <= n; ++i)
            new_cache[i] = new_cache[i-1] + (seq_t)(i-1) * new_cache[i-2];

        cache = new_cache;
        cache_len = new_len;
    }

    /* All cases covered – return the cached value */
    return cache[n];
}
