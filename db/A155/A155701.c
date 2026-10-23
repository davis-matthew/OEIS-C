/*
 * A155701  a(n) = (4^n + 8)/3.
 * https://oeis.org/A155701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 8)/3, equivalently a(0) = 3; a(n) = 4*a(n-1) - 8
 *            for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache if first call */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory, return 0 */
        cache[0] = (seq_t)3;           /* a(0) = 3 */
    }

    /* grow cache to include n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory, return 0 */
        cache = tmp;

        /* compute missing values */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 4*a(i-1) - 8 */
            cache[i] = (seq_t)4 * cache[i - 1] - (seq_t)8;
        }

        cache_size = new_size;
    }

    return cache[n];
}
