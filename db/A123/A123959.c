/*
 * A123959  Expansion of 1/(8*x^5+8*x^4-8*x^3-4*x^2+1).
 * https://oeis.org/A123959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 1
 *   a(2) = 0
 *   a(3) = 4
 *   a(4) = 8
 *   a(5) = 8
 *   for n >= 6
 *       a(n) = 4*a(n-2) + 8*a(n-3) - 8*a(n-4) - 8*a(n-5)
 *   (OEIS offset 1, i.e. a(1) is the first listed term)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms a(1), a(2), ...   */
    static idx_t  cache_len = 0;   /* number of terms stored (>=0)   */
    static size_t cache_cap = 0;   /* allocated capacity              */
    static seq_t *cache = NULL;    /* array of terms (1-indexed)      */

    /* initialise cache with the first five terms */
    if (cache_len == 0) {
        cache_cap = 16;                 /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure; unlikely   */

        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)4;   /* a(3) */
        cache[3] = (seq_t)8;   /* a(4) */
        cache[4] = (seq_t)8;   /* a(5) */
        cache_len = 5;
    }

    /* compute terms up to requested index n */
    if (n > cache_len) {
        /* grow cache if necessary */
        if (n > cache_cap) {
            while (cache_cap < (size_t)n) {
                cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
            }
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            cache = tmp;
        }
        /* compute missing terms */
        for (; cache_len < n; ++cache_len) {
            /* indices are 0-based internally:
             * cache[i] corresponds to a(i+1)
             */
            idx_t i = cache_len;             /* i >=5 -> n >=6 */
            seq_t term1 = (seq_t)4 * cache[i - 2];
            seq_t term2 = (seq_t)8 * cache[i - 3];
            seq_t term3 = (seq_t)8 * cache[i - 4];
            seq_t term4 = (seq_t)8 * cache[i - 5];
            cache[i] = term1 + term2 - term3 - term4;
        }
    }

    /* return the cached value (n >= 1 guaranteed) */
    return cache[n - 1];
}
