/*
 * A042624  Numerators of continued fraction convergents to sqrt(842).
 * https://oeis.org/A042624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 58*a(n-1)+a(n-2), a(0) = 29, a(1) = 1683
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 177 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_cap = 0;    /* number of allocated slots */

    /* Initialise with the first two terms if needed */
    if (cache_cap == 0) {
        cache = mem_new(seq_t, 2);
        if (!cache) { return (seq_t)0; }   /* fallback on allocation failure */
        cache[0] = (seq_t)29;
        cache[1] = (seq_t)1683;
        cache_cap = 2;
    }

    /* Return value if already computed */
    if ((size_t)n < cache_cap) {
        return cache[n];
    }

    /* Grow the cache to accommodate index n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) { return (seq_t)0; }   /* fallback on allocation failure */
    cache = tmp;
    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)58 + cache[i-2];
    }
    cache_cap = new_cap;

    return cache[n];
}
