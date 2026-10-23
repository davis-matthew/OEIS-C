/*
 * A152187  a(n) = 3*a(n-1) + 5*a(n-2), with a(0)=1, a(1)=5.
 * https://oeis.org/A152187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2); a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>  /* memcpy */

seq_t a(idx_t n)
{
    /* static cache for memoised sequence values */
    static seq_t *cache = NULL;       /* array of computed terms   */
    static size_t cache_cap = 0;      /* current capacity in seq_t */
    static idx_t cache_len = 0;       /* number of terms stored    */

    /* grow the cache when needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* failure; should never happen for contest builds */

        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise base cases on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)3 + cache[i - 2] * (seq_t)5;
    }
    cache_len = n + 1;   /* now all terms up to n are stored */

    return cache[n];
}
