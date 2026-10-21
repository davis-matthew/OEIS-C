/*
 * A231679  a(n) = Sum_{i=0..n} digsum_7(i)^4, where digsum_7(i) = A053828(i).
 * https://oeis.org/A231679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_7(i)^4 digsum_7(i) = sum of digits of
 *            i written in base 7
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum_base7.h"

seq_t a(idx_t n)
{
    /* static cache of a(0)..a(k) for the largest k seen so far */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;         /* number of valid entries in cache */
    static idx_t cache_cap = 0;         /* allocated capacity of cache */

    /* ensure cache can hold a(0)..a(n) */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values up to n if not yet cached */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        if (k == 0)
        {
            cache[0] = 0;
        }
        else
        {
            seq_t prev = cache[k - 1];
            seq_t d = digit_sum_base7(k);
            seq_t d2 = d * d;
            seq_t d4 = d2 * d2;
            cache[k] = prev + d4;
        }
    }
    cache_len = n + 1;

    return cache[n];
}
