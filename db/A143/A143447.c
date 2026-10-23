/*
 * A143447  Expansion of 1/(x^k*(1-x-2*x^(k+1))) for k=4.
 * https://oeis.org/A143447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n+1 for 0 ≤ n ≤ 5 * a(n) = a(n-1) + 2*a(n-5) for n ≥ 6
 *            * The function memoises previously computed values so that each
 *            index * is evaluated at most once, independent of call order
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 57, u64 n <= 117,
 *            u128 n <= 236, big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoisation array */
    static idx_t len   = 0;         /* number of computed elements */
    static idx_t cap   = 0;         /* allocated capacity */

    /* initialise when first called */
    if (cache == NULL) {
        cap   = 6;                      /* at least indices 0..5 are needed */
        cache = seq_alloc_array(cap);
    }

    /* compute any missing terms up to n */
    while (len <= n) {
        /* grow capacity if necessary */
        if (len == cap) {
            idx_t new_cap = (cap == 0) ? 6 : cap * 2;
            if (new_cap <= len) new_cap = len + 1;
            seq_t *tmp = seq_alloc_array(new_cap);
            for (idx_t i = 0; i < len; ++i) tmp[i] = cache[i];
            free(cache);
            cache = tmp;
            cap   = new_cap;
        }

        /* compute cache[len] */
        if (len <= 5) {
            /* 2*len + 1, cast to seq_t first to avoid overflow of idx_t */
            cache[len] = (seq_t)( (seq_t)2 * (seq_t)len + (seq_t)1 );
        } else {
            cache[len] = cache[len-1] + (seq_t)2 * cache[len-5];
        }
        ++len;
    }
    return cache[n];
}
