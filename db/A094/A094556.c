/*
 * A094556  Number of walks of length n between opposite vertices on a
 *          triangular prism.
 * https://oeis.org/A094556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+5*a(n-2)-6*a(n-3), initial values: a(0)=0,
 *            a(1)=1, a(2)=0, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;      /* number of allocated slots   */
    static idx_t last_computed = 3;  /* maximal index whose value is known */

    /* Create initial cache on first call */
    if (cache_len == 0) {
        cache_len = 4;  /* indices 0..3 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = 7;
    }

    /* If the requested value is already known, return it. */
    if (n <= last_computed)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;   /* double until big enough */

        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * 2 + cache[i - 2] * 5;
        cache[i] -= cache[i - 3] * 6;
    }

    last_computed = n;
    return cache[n];
}
