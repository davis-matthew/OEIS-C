/*
 * A041795  Denominators of continued fraction convergents to sqrt(418).
 * https://oeis.org/A041795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 67714*a(n-6) - a(n-12) for n > 11 Initial values: a(0)=1,
 *            a(1)=2, a(2)=9, a(3)=182, a(4)=737, a(5)=1656, a(6)=66977,
 *            a(7)=135610, a(8)=609417, a(9)=12323950, a(10)=49905217,
 *            a(11)=112134384
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;
    static const seq_t init[12] = {
        1,2,9,182,737,1656,
        66977,135610,609417,12323950,49905217,112134384
    };

    /* Expand cache if needed */
    if (n >= cached_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = seq_alloc((size_t)new_size);
        if (cache)
        {
            /* copy old values */
            for (idx_t i = 0; i < cached_size; ++i)
                new_cache[i] = cache[i];
            /* old memory is not freed – acceptable leak for short-lived programs */
        }
        /* Compute new entries */
        for (idx_t i = cached_size; i <= n; ++i)
        {
            if (i < 12)
                new_cache[i] = init[i];
            else
                new_cache[i] = (seq_t)67714 * new_cache[i-6] - new_cache[i-12];
        }
        cache = new_cache;
        cached_size = new_size;
    }
    return cache[n];
}
