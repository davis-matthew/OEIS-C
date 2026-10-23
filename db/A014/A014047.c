/*
 * A014047  Inverse of 38th cyclotomic polynomial.
 * https://oeis.org/A014047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = Σ_{k=1}^{18} (-1)^{k+1} · a(n-k)
 *            (Indices < 0 are treated as 0.) This recurrence follows from
 *            1/(1 − x + x² − … + x¹⁸)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms        */
static size_t cache_cap = 0;         /* allocated capacity             */
static idx_t  cache_max = (idx_t)-1; /* highest index already computed */

seq_t a(idx_t n) {
    /* Ensure the cache is initialized. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* fallback if allocation fails   */
        cache[0] = 1;
        cache_max = 0;
    }

    /* Grow the cache if needed. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* fallback on failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 18; ++k) {
            if (i >= k) {
                seq_t term = cache[i - k];
                /* (-1)^{k+1} is +1 for odd k, -1 for even k. */
                sum += (k & 1) ? term : -term;
            }
        }
        cache[i] = sum;
    }
    cache_max = n;
    return cache[n];
}
