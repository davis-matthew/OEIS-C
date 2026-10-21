/*
 * A218744  a(n) = (41^n - 1)/40.
 * https://oeis.org/A218744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (41^n - 1)/40, equivalently a(0)=0, a(1)=1,
 *            a(n)=42*a(n-1)-41*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* next index that needs to be computed */

    /* Ensure the cache has room for indices up to n. */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) {
            abort();   /* allocation failed */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_len up to n (inclusive) if not already done. */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                cache[1] = 1;
            } else {
                /* recurrence: a(n) = 42*a(n-1) - 41*a(n-2) */
                cache[i] = (42 * cache[i - 1]) - (41 * cache[i - 2]);
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
