/*
 * A130707  a(n+3) = 3*(a(n+2) - a(n+1)) + 2*a(n).
 * https://oeis.org/A130707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 3*(a(n+2)-a(n+1)) + 2*a(n), a(0)=1, a(1)=2, a(2)=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to avoid recomputation for subsequent calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first three terms if empty */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;          /* allocation failure, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
    }

    /* enlarge cache if needed */
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return (seq_t)0;      /* allocation failure, return 0 */
        cache = new_cache;
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) continue;            /* already initialised */
            if (i == 1) continue;
            if (i == 2) continue;
            /* a(i) = 3 * (a(i-1) - a(i-2)) + 2 * a(i-3) */
            seq_t term = (seq_t)3 * ((seq_t)cache[i - 1] - (seq_t)cache[i - 2])
                          + (seq_t)2 * (seq_t)cache[i - 3];
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
