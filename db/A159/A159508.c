/*
 * A159508  Numerator of Hermite(n, 3/14).
 * https://oeis.org/A159508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7^n * Hermite(n, 3/14) Recurrence: a(0) = 1, a(1) = 3
 *            a(n) = 3 * a(n-1) - 98 * (n-1) * a(n-2) for n ≥ 2 This uses the
 *            physics‐Hermite recurrence H_n(x) = 2x H_{n-1}(x) – 2(n-1)
 *            H_{n-2}(x) with 2x = 3/7. The recurrence yields integer values
 *            when multiplied by 7^n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t   cache_size = 0;   /* first unused index  (cache size) */
    static size_t   cache_cap  = 0;   /* allocated capacity           */

    /* ensure base values are present */
    if (cache == NULL) {
        cache_cap   = 2;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* OOM – will never happen for test cases */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)3;           /* a(1) */
        cache_size = 2;
    }

    /* grow cache if necessary (doubling strategy) */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;        /* new_cap *= 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                         /* OOM */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 98*(i-1)*a(i-2) */
        seq_t coeff = (seq_t)98 * (seq_t)(i - 1);
        cache[i] = (seq_t)3 * cache[i-1] - coeff * cache[i-2];
    }
    cache_size = (size_t)n + 1;   /* now cache contains a(0)…a(n) */

    return cache[(size_t)n];
}
