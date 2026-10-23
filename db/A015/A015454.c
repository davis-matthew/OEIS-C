/*
 * A015454  Generalized Fibonacci numbers.
 * https://oeis.org/A015454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2); a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;          /* array of computed values   */
    static idx_t cached_max = (idx_t)-1;    /* sentinel: uninitialised    */

    /* initialise cache for the first call */
    if (cached_max == (idx_t)-1) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        if (!cache) exit(1);                /* allocation failed          */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cached_max = 1;
    }

    /* if the requested value is already cached, return it */
    if (n <= cached_max)
        return cache[n];

    /* otherwise extend the cache up to the requested index */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                       /* realloc failed           */
    cache = tmp;

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i - 1] + cache[i - 2];
    }
    cached_max = n;

    return cache[n];
}
