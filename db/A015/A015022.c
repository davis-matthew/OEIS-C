/*
 * A015022  q-factorial numbers for q=-8.
 * https://oeis.org/A015022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = ((-8)^n - 1)*a(n-1)/(-9) (n >= 1)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* computed terms cache */
    static idx_t cache_size = 0;       /* number of valid entries in cache */
    static seq_t last_power = 0;       /* last computed power (-8)^k, k = cache_size-1 */

    /* initialise cache for n = 0 */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, return 0 */
        cache[0] = (seq_t)1;
        cache_size = 1;
        last_power = (seq_t)1;          /* (-8)^0 */
    }

    /* extend cache to required index n */
    if (cache_size <= n) {
        /* enlarge array */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;      /* allocation failure */
        cache = new_cache;

        /* compute terms from cache_size to n (inclusive) */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* update power to (-8)^i */
            last_power = last_power * (seq_t)(-8);

            /* compute factor = ((-8)^i - 1) / (-9) */
            seq_t factor = (last_power - (seq_t)1) / (seq_t)(-9);

            /* compute a(i) = a(i-1) * factor */
            cache[i] = cache[i - 1] * factor;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
