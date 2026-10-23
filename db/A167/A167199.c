/*
 * A167199  First column of A167196.
 * https://oeis.org/A167199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(1)=-2; for n>=2, a(n)=-(4*n*a(n-1)+n*(n-1)*a(n-2))/2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* ensure cache has space up to n */
    size_t needed = (size_t)n + 1;
    if (needed < 2)          /* base values need space for index 1 at least */
        needed = 2;
    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? needed : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base values if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache_len = 2;
    } else if (cache_len == 1) {     /* possible if n==0 only */
        cache[1] = (seq_t)-2;
        cache_len = 2;
    }

    /* fill cache up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence for i >= 2 */
            seq_t term1 = 4 * (seq_t)i * cache[i - 1];
            seq_t term2 = (seq_t)i * ((seq_t)(i - 1)) * cache[i - 2];
            cache[i] = -(term1 + term2) / 2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
