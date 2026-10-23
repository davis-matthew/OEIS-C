/*
 * A159647  Numerator of Hermite(n, 9/19).
 * https://oeis.org/A159647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19^n * Hermite(n, 9/19) * Recurrence: a(0) = 1, a(1) = 18
 *            * a(n) = 18*a(n-1) - 722*(n-1)*a(n-2) (for n ≥ 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 10, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed values   */
    static idx_t cache_size = 0;         /* current capacity            */
    static idx_t cache_count = 0;        /* number of initialized items */

    /* allocate / grow the cache as needed */
    if (cache == NULL) {
        /* initial size at least 2 to hold a(0) and a(1) */
        cache_size = (n + 1 > 2 ? (n + 1) : 2);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return 0;      /* OOM – graceful failure */
    } else if (n + 1 > cache_size) {
        /* grow by doubling until large enough */
        idx_t new_size = cache_size;
        while (n + 1 > new_size) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise the first two terms if they are not yet set */
    if (cache_count == 0) {
        cache[0] = (seq_t)1;
        cache_count = 1;
    }
    if (cache_count == 1) {
        cache[1] = (seq_t)18;
        cache_count = 2;
    }

    /* already have the requested value? */
    if (n < cache_count) return cache[n];

    /* compute further terms up to n */
    for (idx_t i = cache_count; i <= n; ++i) {
        seq_t term = (seq_t)18 * cache[i - 1];
        term -= (seq_t)722 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term;
    }
    cache_count = n + 1;

    return cache[n];
}
