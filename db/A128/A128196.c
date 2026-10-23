/*
 * A128196  a(n) = (2*n - 1)*a(n - 1) + 2^n for n >= 1, a(0) = 1.
 * https://oeis.org/A128196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n - 1) * a(n - 1) + 2^n for n >= 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>            /* malloc, realloc, abort */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;

    /* Ensure the cache is large enough for indices 0..n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + (size_t)1;
        if (new_cap < 16) new_cap = 16;               /* small start size */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                                  /* allocation failure */

        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;                              /* initialise new slots */

        cache_cap = new_cap;
    }

    /* Initialise a(0) if this is the first call */
    if (cache[0] == 0)
        cache[0] = (seq_t)1;

    /* Compute values only when they have not been computed yet. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] == 0) {
            seq_t prev   = cache[i - 1];
            seq_t term   = (seq_t)(2 * i - 1) * prev;  /* (2i‑1)·a(i‑1) */
            term += pow2(i);                           /* + 2^i */
            cache[i] = term;
        }
    }

    return cache[n];
}
