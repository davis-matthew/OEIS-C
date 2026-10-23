/*
 * A159650  Numerator of Hermite(n, 12/19).
 * https://oeis.org/A159650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 24 Recurrence: a(n) = 24*a(n-1) -
 *            722*(n-1)*a(n-2) (n ≥ 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache holds a(0)..a(max_computed) */
    static seq_t *cache;
    static size_t cache_cap;
    static size_t cache_len;

    /* initialise cache if this is the first call */
    if (!cache) {
        cache_cap = (n > 1 ? (size_t)n + 1 : 2U);
        /* allocate array – use C++ new when available */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) return 0;                    /* allocation failed – guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache_len = 2;
    }
    /* grow cache if needed */
    else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* reallocation failed – guard */
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term1 = cache[i-1] * ((seq_t)24);
        seq_t prod   = ((seq_t)722) * ((seq_t)(i - 1));
        cache[i] = term1 - prod * cache[i-2];
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
