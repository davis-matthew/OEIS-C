/*
 * A168569  a(n) = 9*(3^n - 1)/2.
 * https://oeis.org/A168569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*(3^n - 1)/2 Recurrence: a(0) = 0, a(1) = 9, a(n) =
 *            4*a(n-1) - 3*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* 0‑indexed cache of computed terms   */
    static size_t cache_cap = 0;    /* number of elements that are valid   */

    /* Ensure the cache contains the base cases a(0)=0, a(1)=9. */
    if (cache_cap < 2) {
        size_t new_cap = 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)9;
        cache_cap = new_cap;
    }

    /* If the requested index is already cached, return it. */
    if ((size_t)n < cache_cap) {
        return cache[n];
    }

    /* Grow the cache to accommodate index n. */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;

    /* Compute all missing terms using the recurrence. */
    for (size_t i = cache_cap; i <= (size_t)n; ++i) {
        cache[i] = 4 * cache[i - 1] - 3 * cache[i - 2];
    }
    cache_cap = new_cap;

    return cache[n];
}
