/*
 * A026599  a(n) = Sum_{j=0..2*i, i=0..n} A026584(i,j).
 * https://oeis.org/A026599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=3; a(2)=9; For n≥3: a(n)=2*a(n-1)+3*a(n-2)-4*a(n-3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) for 0 ≤ i < max_computed */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;  /* number of cached values */

    /* enlarge cache if needed */
    if (n + 1 > max_computed)
    {
        /* allocate new larger cache */
        idx_t new_len = n + 1;
        seq_t *new_cache = alloc_cache(new_len);
        /* copy existing values to new cache */
        if (cache)
        {
            for (idx_t i = 0; i < max_computed; ++i)
                new_cache[i] = cache[i];
            free_cache(cache);
        }
        cache = new_cache;

        /* compute any new terms */
        for (idx_t k = max_computed; k <= n; ++k)
        {
            if (k == 0)
                cache[0] = (seq_t)1;
            else if (k == 1)
                cache[1] = (seq_t)3;
            else if (k == 2)
                cache[2] = (seq_t)9;
            else
            {
                seq_t v1 = cache[k - 1];
                seq_t v2 = cache[k - 2];
                seq_t v3 = cache[k - 3];
                cache[k] = v1 * 2 + v2 * 3 - v3 * 4;
            }
        }
        max_computed = new_len;
    }

    return cache[n];
}
