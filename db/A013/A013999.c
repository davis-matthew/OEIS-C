/*
 * A013999  From applying the "rational mean" to the number e.
 * https://oeis.org/A013999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013999 (OEIS)
   Recurrence:
      a(0) = 1, a(1) = 1, a(2) = 2
      a(n) = (n+3)*a(n-1) - (2n+1)*a(n-2) + n*a(n-3)   for n ≥ 3
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache        = NULL; /* array of terms */
    static idx_t cache_cap    = 0;      /* allocated size   */
    static idx_t cache_filled = 0;      /* first unused idx */

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialize the first terms */
    if (cache_filled == 0) {
        cache[0] = 1;
        cache_filled = 1;
    }
    if (cache_filled == 1 && n >= 1) {
        cache[1] = 1;
        cache_filled = 2;
    }
    if (cache_filled == 2 && n >= 2) {
        cache[2] = 2;
        cache_filled = 3;
    }

    /* compute missing terms up to n */
    if (cache_filled <= n) {
        for (idx_t i = cache_filled; i <= n; ++i) {
            seq_t t1 = (seq_t)(i + 3)   * cache[i - 1];
            seq_t t2 = (seq_t)(2 * i + 1) * cache[i - 2];
            seq_t t3 = (seq_t)(i)       * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
        cache_filled = n + 1;
    }

    return cache[n];
}
