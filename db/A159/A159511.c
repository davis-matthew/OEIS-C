/*
 * A159511  Numerator of Hermite(n, 11/14).
 * https://oeis.org/A159511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7^n * Hermite(n, 11/14) Recurrence: a(0) = 1, a(1) = 11;
 *            a(n) = 11*a(n-1) - 98*(n-1)*a(n-2) (n ≥ 2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* initialise cache with the first two values */
    if (cache == NULL) {
        cache_cap  = 2;
        cache_size = 2;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure: fall back to returning 0 (should never happen) */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
    }

    /* return if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow array if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* realloc failure: keep old cache, do nothing */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        /* Use 128‑bit intermediate if seq_t is a built‑in numeric type */
        if (sizeof(seq_t) <= sizeof(long long)) {
            __int128 t = (__int128)11 * (__int128)prev1
                       - (__int128)98 * (__int128)(i - 1) * (__int128)prev2;
            cache[i] = (seq_t)t;
        } else {
            cache[i] = 11 * prev1 - 98 * ((seq_t)(i - 1) * prev2);
        }
    }
    cache_size = n + 1;

    return cache[n];
}
