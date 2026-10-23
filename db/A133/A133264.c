/*
 * A133264  Smallest number whose sum of digits is 3n+1.
 * https://oeis.org/A133264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + 10*a(n-3) - 10*a(n-4) Initial values: a(0)=1,
 *            a(1)=4, a(2)=7, a(3)=19 Uses an expanding cache to allow
 *            arbitrary order of calls
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Handle the first four terms directly */
    if (n < 4) {
        /* The values fit in any seq_t type. */
        switch ((size_t)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)4;
        case 2: return (seq_t)7;
        default: return (seq_t)19;   /* n == 3 */
        }
    }

    /* Ensure the cache contains at least the first four terms. */
    if (!cache) {
        size_t init = (size_t)(n + 1);
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure sentinel */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)19;
        cache_size = 4;
    }

    /* Expand the cache if necessary. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure sentinel */
        cache = tmp;

        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            cache[i] =
                cache[i - 1] +
                (seq_t)10 * cache[i - 3] -
                (seq_t)10 * cache[i - 4];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
