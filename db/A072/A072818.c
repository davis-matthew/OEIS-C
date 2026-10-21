/*
 * A072818  Expansion of g.f. 20*x/(1-98*x+x^2).
 * https://oeis.org/A072818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 20, a(n) = 98*a(n-1) - a(n-2) for n > 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;          /* pointer to allocated array */
    static size_t cache_size = 0;        /* number of cached terms */

    /* Initialise cache to hold at least the first two terms */
    if (cache_size == 0) {
        cache_size = 2;                  /* we store a(0) and a(1) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory → return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)20;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* out‑of‑memory → return 0 */
        cache = new_cache;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_size; i < new_size; ++i) {
            /* Cast operands to seq_t to stay within allowed operations */
            cache[i] = (seq_t)98 * cache[i-1] - cache[i-2];
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
