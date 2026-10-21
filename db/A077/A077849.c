/*
 * A077849  Expansion of (1-x)^(-1)/(1 - 2*x - x^2 - x^3).
 * https://oeis.org/A077849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-4) Initial values: a(0)=1,
 *            a(1)=3, a(2)=8, a(3)=21 Recurrence holds for n ≥ 4. The function
 *            memoizes the results so that calls with arbitrary n can be
 *            answered in O(n) time
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of stored elements */

    /* ensure the cache can hold the required index */
    if (cache_size == 0) {
        /* initialise with the first four terms */
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)21;
    }

    if (n + 1 > cache_size) {
        /* grow the cache to at least double the needed size or the
           required size, whichever is larger */
        size_t new_size = (n + 1) * 2;
        if (new_size < n + 1) new_size = n + 1;     /* safety for overflows */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failed: return 0 */
        cache = tmp;
        /* compute terms from the current size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence valid for i>=4; for i<4 the base terms are already set */
            cache[i] = 3 * cache[i - 1] - cache[i - 2] - cache[i - 4];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
