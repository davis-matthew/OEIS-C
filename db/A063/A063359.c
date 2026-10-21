/*
 * A063359  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          86 ).
 * https://oeis.org/A063359
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13. Initial values:
 *            a(2) = -1, a(3) = 76, a(4) = 154, a(5) = 231, a(6) = 308, a(7) =
 *            385, a(8) = 462, a(9) = 537, a(10) = 616, a(11) = 691, a(12) =
 *            770, a(13) = 847
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"    /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t*,size_t,size_t) */
#include <stddef.h>                /* size_t */

seq_t a(idx_t n)
{
    /* undefined for n < 2 – return 0 */
    if (n < 2) return 0;

    static seq_t *cache = NULL;   /* memoised values */
    static size_t cached_cap = 0; /* allocated capacity */
    static idx_t cached_max = 0;  /* highest n computed so far */

    /* initial values for indices 0 .. 13 */
    static const seq_t init[14] = {
        0, 0,          /* 0,1 unused */
        -1, 76, 154, 231, 308, 385, 462, 537,
        616, 691, 770, 847
    };

    /* return directly for n ≤ 13 */
    if (n <= 13) return init[(size_t)n];

    /* initialize the cache on first call */
    if (cache == NULL) {
        cached_cap = 14;
        cache = seq_alloc(cached_cap);
        if (cache == NULL) return 0;      /* allocation failure */
        for (size_t i = 0; i < cached_cap; ++i)
            cache[i] = init[i];
        cached_max = 13;
    }

    /* enlarge cache if not large enough */
    if ((size_t)n + 1 > cached_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = seq_realloc(cache, cached_cap, new_cap);
        if (new_cache == NULL) return 0;  /* allocation failure */
        cache = new_cache;
        for (size_t i = cached_cap; i < new_cap; ++i)
            cache[i] = 0;
        cached_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        cache[idx] = cache[idx - 4] + cache[idx - 6] - cache[idx - 10];
    }
    cached_max = n;

    return cache[(size_t)n];
}
