/*
 * A023369  a(n+1) = a(n) converted to base 6 from base 3 (written in base
 *          10).
 * https://oeis.org/A023369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=3, a(n+1)=number obtained by interpreting the base‑3 digits
 *            of a(n) * as base‑6 digits (all digits are 0,1,2). The sequence
 *            is computed * iteratively and cached so that calls can arrive in
 *            any order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   base3_to_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/base3_to_base6.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cache previous terms.  We use 1‑based indexing for the cache
     * so that cache[i-1] holds a(i).  cache[0] is a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == (idx_t)0) return (seq_t)0; /* undefined for offset 1 */

    if (cache_len < n)
    {
        /* Grow cache to size n */
        seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failed */
        cache = new_cache;

        /* Compute terms up to n */
        for (idx_t i = cache_len; i < n; ++i)
        {
            if (i == (idx_t)0)
            {
                cache[i] = (seq_t)3; /* a(1) = 3 */
            }
            else
            {
                cache[i] = base3_to_base6(cache[i - 1]); /* a(i+1) = f(a(i)) */
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
