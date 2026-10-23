/*
 * A028051  Expansion of 1/((1-3x)(1-4x)(1-10x)(1-12x)).
 * https://oeis.org/A028051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 286*a(n-2) + 1104*a(n-3) - 1440*a(n-4)
 *            Initial values: a(0)=1, a(1)=29, a(2)=555, a(3)=8905
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cap = 0;

seq_t a(idx_t n)
{
    /* Handle trivial small indices */
    if (n < 4) {
        static const seq_t init[4] = { (seq_t)1, (seq_t)29, (seq_t)555, (seq_t)8905 };
        return init[(size_t)n];
    }

    /* Determine required buffer size */
    const size_t required = (size_t)(n + 1);
    if (required > cap) {
        /* old cap before resizing */
        const size_t old_cap = cap;

        /* Allocate or grow buffer */
        seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failure → undefined result */

        if (cache) {
            /* copy previously computed terms */
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }

        /* initialize new slots to zero */
        for (size_t i = cap; i < required; ++i)
            new_cache[i] = (seq_t)0;

        /* ensure the first four terms exist */
        new_cache[0] = (seq_t)1;
        new_cache[1] = (seq_t)29;
        new_cache[2] = (seq_t)555;
        new_cache[3] = (seq_t)8905;

        /* swap cache and update capacity */
        cache = new_cache;
        cap   = required;

        /* compute missing terms only */
        const size_t start = old_cap > 4 ? old_cap : 4;
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t t = (seq_t)29 * cache[i - 1]
                    - (seq_t)286 * cache[i - 2]
                    + (seq_t)1104 * cache[i - 3]
                    - (seq_t)1440 * cache[i - 4];
            cache[i] = t;
        }
    }

    return cache[(size_t)n];
}
