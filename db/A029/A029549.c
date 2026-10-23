/*
 * A029549  a(n + 3) = 35*a(n + 2) - 35*a(n + 1) + a(n), with a(0) = 0, a(1) =
 *          6, a(2) = 210.
 * https://oeis.org/A029549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 35*a(n+2) - 35*a(n+1) + a(n) with a(0) = 0, a(1) = 6,
 *            a(2) = 210
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache      = NULL;   /* array of a(i) */
    static idx_t  cache_size = 0;      /* allocated length   */
    static idx_t  max_n      = (idx_t)-1;   /* highest index computed */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)210;
        max_n    = 2;
    }

    /* value already computed? */
    if (n <= max_n) return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 16;
        while (new_size <= (size_t)n) new_size *= 2;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val =
            (seq_t)35 * cache[i - 1]   /* 35*a(i-1)   */
            - (seq_t)35 * cache[i - 2] /* -35*a(i-2)  */
            +          cache[i - 3];   /* + a(i-3)    */
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
