/*
 * A042300  Numerators of continued fraction convergents to sqrt(677).
 * https://oeis.org/A042300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 52*a(n-1) + a(n-2) for n > 1, a(0) = 26, a(1) = 1353
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;

    /* initialise cache */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed – unlikely */
        cache[0] = 26;
        cache[1] = 1353;
        max_computed = 1;
    }

    /* trivial cases */
    if (n == 0) return 26;
    if (n == 1) return 1353;

    /* extend cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed – unlikely */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)52 * cache[i - 1] + cache[i - 2];
    }
    max_computed = n;

    return cache[n];
}
