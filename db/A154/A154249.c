/*
 * A154249  a(n) = ( (8 + sqrt(7))^n - (8 - sqrt(7))^n )/(2*sqrt(7)).
 * https://oeis.org/A154249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 57*a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 745 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization: cache[i] holds a(i) for i = 0..cache_len-1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements currently stored (>= n+1 when n has been computed) */

    /* Initialise cache with the first three terms: a(0), a(1), a(2) */
    if (cache == NULL) {
        cache_len = 3;                              /* indices 0,1,2 */
        cache = mem_new(seq_t, cache_len);
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = 0;                               /* a(0) */
        cache[1] = 1;                               /* a(1) */
        cache[2] = 16;                              /* a(2) */
    }

    /* If requested n is beyond the cached range, extend the cache */
    if (n >= cache_len) {
        idx_t new_len = n + 1;        /* we need indices 0..n inclusive */
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;

        /* Compute new terms using the linear recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 16 * cache[i-1] - 57 * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
