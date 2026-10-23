/*
 * A160072  Numerator of Hermite(n, 7/26).
 * https://oeis.org/A160072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n, 7/26) Recurrence: a(0) = 1, a(1) = 7,
 *            a(n) = 7*a(n-1) - 338*(n-1)*a(n-2) for n ≥ 2 (n is an idx_t;
 *            seq_t is the integer type of the build.)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* One‑time cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* number of elements allocated */
    static size_t cache_len = 0;      /* indices computed (next free) */

    /* Ensure the cache covers index 0 and 1 */
    if (cache == NULL) {
        cache_cap = 4;                      /* start with a few slots */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_len = 2;
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend the sequence up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; continue; }
        if (i == 1) { cache[i] = (seq_t)7; continue; }

        /* a(i) = 7*a(i-1) - 338*(i-1)*a(i-2) */
        seq_t term1 = (seq_t)7 * cache[i-1];
        seq_t term2 = (seq_t)338 * ((seq_t)(i-1)) * cache[i-2];
        cache[i] = term1 - term2;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
