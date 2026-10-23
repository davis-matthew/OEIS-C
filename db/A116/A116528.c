/*
 * A116528  a(0)=0, a(1)=1, and for n>=2, a(2*n) = a(n), a(2*n+1) = 2*a(n) +
 *          a(n+1).
 * https://oeis.org/A116528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; for n>=2 : a(2*n)=a(n), a(2*n+1)=2*a(n)+a(n+1)
 * Range:     u8 n <= 170, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                 /* for idx_t, uint64_t */
#include "../utils/seq_alloc.h"     /* seq_alloc(size_t) -> seq_t* */
#include "../utils/seq_realloc.h"   /* seq_realloc(void*,size_t,size_t) -> void* */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* allocated array capacity */
    static idx_t cache_max = 0;     /* last index that has been filled */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                          /* at least indices 0 and 1 */
        cache = (seq_t *)seq_alloc(cache_cap);
        if (cache == NULL) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)0;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* grow cache if necessary */
    if (n > cache_max) {
        size_t new_cap = cache_cap;
        /* exponential growth to minimise reallocations */
        while ((idx_t)new_cap <= n) {
            new_cap *= 2;
            if (new_cap == 0) {            /* overflow guard */
                new_cap = (size_t)n + 1;
                break;
            }
        }
        if (new_cap <= cache_cap) new_cap = (size_t)n + 1;

        seq_t *tmp = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) return cache[n]; /* fallback if realloc failed */
        cache = tmp;
        cache_cap = new_cap;

        /* compute new values sequentially */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if ((i & 1) == 0) {            /* even index */
                cache[i] = cache[i >> 1];
            } else {                      /* odd index */
                cache[i] = 2 * cache[i >> 1] + cache[(i >> 1) + 1];
            }
        }
        cache_max = n;
    }

    /* return the requested term */
    return cache[n];
}
