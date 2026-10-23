/*
 * A160435  Numerator of Hermite(n, 21/32).
 * https://oeis.org/A160435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 21 recurrence: a(n) = 21*a(n-1) -
 *            512*(n-1)*a(n-2) for n >= 2 (these are the numerators of
 *            H_n(21/32) where H_n is the physicists' Hermite polynomial)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* cache capacity in terms of terms stored */
    static size_t computed = 0;   /* number of terms already filled in cache */

    /* ensure the cache can hold up to index n */
    ensure_cache(n, &cache, &capacity);

    /* initialise base cases if not already present */
    if (computed < 1) {
        cache[0] = 1;
        computed = 1;
    }
    if (computed < 2) {
        cache[1] = 21;
        computed = 2;
    }

    /* if the requested term is already computed, return it immediately */
    if (n < computed)
        return cache[n];

    /* compute missing terms in order, using the recurrence */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t term1 = 21 * cache[i - 1];
        seq_t term2 = 512 * ((seq_t)(i - 1)) * cache[i - 2];
        cache[i] = term1 - term2;
    }

    /* update the count of computed terms */
    computed = (size_t)n + 1;

    return cache[n];
}
