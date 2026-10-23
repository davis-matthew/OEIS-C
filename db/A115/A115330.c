/*
 * A115330  E.g.f: exp(x/(1-4*x))/sqrt(1-16*x^2).
 * https://oeis.org/A115330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-3)*a(n-1) + 4*(n-1)*(4*n-3)*a(n-2) -
 *            64*(n-1)*(n-2)^2*a(n-3) a(0) = 1, a(1) = 1, a(2) = 25
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache      = NULL;   /* array of a(i) */
    static idx_t  cache_used = 0;      /* number of elements filled so far */
    static idx_t  cache_cap  = 0;      /* current allocated size (array length) */

    /* Grow cache if necessary */
    if (cache_cap < (idx_t)(n + 1)) {
        idx_t new_cap = (idx_t)(n + 1);
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache) return 0;   /* allocation failed – return zero (unlikely) */

        /* copy existing values */
        for (idx_t i = 0; i < cache_used; ++i) {
            new_cache[i] = cache[i];
        }

        /* free old cache */
        if (cache != NULL) {
            seq_free_array(cache);
        }

        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_used; k <= n; ++k) {
        switch (k) {
        case 0:
            cache[0] = 1;
            break;
        case 1:
            cache[1] = 1;
            break;
        case 2:
            cache[2] = 25;
            break;
        default: {
            seq_t term1 = (seq_t)(4 * k - 3) * cache[k - 1];
            seq_t term2 = (seq_t)4 * (k - 1) * (seq_t)(4 * k - 3) * cache[k - 2];
            seq_t term3 = (seq_t)64 * (k - 1) * (k - 2) * (k - 2) * cache[k - 3];
            cache[k] = term1 + term2 - term3;
            break;
        }
        }
    }

    /* update number of initialized elements */
    cache_used = n + 1;

    return cache[n];
}
