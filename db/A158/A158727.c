/*
 * A158727  a(n) = Hermite(n, 21).
 * https://oeis.org/A158727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 42*a(n-1) - 2*(n-1)*a(n-2), a(0)=1, a(1)=42
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the sequence A158727: Hermite(n,21). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_cached = (idx_t)-1;  /* indicates uninitialised */

    /* Compute required storage size. */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) { /* allocation failed – aborting is acceptable for a single call */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = need;
    }

    /* First‑time initialisation */
    if (last_cached == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (n >= 1)
            cache[1] = (seq_t)42;
        last_cached = (n >= 1) ? (idx_t)1 : (idx_t)0;
    }

    /* If the requested n is already cached, return it. */
    if (n <= last_cached)
        return cache[n];

    /* Compute missing terms up to n. */
    const seq_t c42 = (seq_t)42;
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        seq_t term1 = c42 * cache[i - 1];
        seq_t factor = (seq_t)(2ULL * (i - 1));
        seq_t term2 = factor * cache[i - 2];
        cache[i] = term1 - term2;
    }
    last_cached = n;
    return cache[n];
}
