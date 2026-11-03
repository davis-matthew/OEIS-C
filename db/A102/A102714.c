/*
 * A102714  Expansion of (x+2) / ((x+1)*(x^2-3*x+1)).
 * https://oeis.org/A102714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 992 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A102714
   Formula: a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3)
   Initial values: a(0) = 2, a(1) = 5, a(2) = 14
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static idx_t cached = 0;      /* number of elements stored in cache */

    /* Initialise cache with the first three terms. */
    if (cache == NULL) {
        cache   = seq_malloc(3);
        cache[0] = 2;
        cache[1] = 5;
        cache[2] = 14;
        cached = 3;
    }

    /* Return directly if the value is already cached. */
    if (n < cached)
        return cache[n];

    /* Enlarge the cache to accommodate up to term n. */
    idx_t new_size = n + 1;
    cache = seq_realloc(cache, new_size);

    /* Compute forward from the first missing index up to n. */
    for (idx_t i = cached; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + 2 * cache[i-2] - cache[i-3];
    }

    cached = new_size;
    return cache[n];
}
