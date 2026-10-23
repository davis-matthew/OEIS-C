/*
 * A159286  Expansion of (x-1)^2/(1-x^2-2*x^3).
 * https://oeis.org/A159286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) + 2*a(n-3) for n >= 3, with a(0)=1, a(1)=-2,
 *            a(2)=2
 * Range:     i8 n <= 18, i16 n <= 32, i32 n <= 58, i64 n <= 111,
 *            i128 n <= 217, big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size of cache */
    static idx_t last  = 0;   /* highest index for which cache is filled */

    /* Ensure the cache can hold n */
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        if (new_cap <= n) new_cap = n + 1;
        if (new_cap < 3) new_cap = 3;   /* we need space for at least 3 elements */

        seq_t *new_cache = seq_alloc_array(new_cap);
        if (!new_cache) return 0;       /* allocation failure – return 0 as placeholder */

        for (idx_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];    /* copy existing values */

        if (cache)
            free_seq_array(cache);      /* free old array */

        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialize first three terms if not already done */
    if (last < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)2;
        last = 2;
    }

    /* Compute terms up to n (if needed) */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + (seq_t)2 * cache[i - 3];
    }
    last = n;

    return cache[n];
}
