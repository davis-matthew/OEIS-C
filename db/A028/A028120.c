/*
 * A028120  Expansion of 1/((1-4x)(1-5x)(1-8x)(1-9x)).
 * https://oeis.org/A028120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 245*a(n-2) + 988*a(n-3) - 1440*a(n-4), n ≥ 4
 *            * with initial values: a(0)=1, a(1)=26, a(2)=431, a(3)=5824. * *
 *            The function uses a static cache so that any order of calls is
 *            handled * and each term is computed only once. The recurrence is
 *            evaluated * using unsigned arithmetic; i...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache and its lengths are local to the function */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* if n is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate n */
    size_t need = n + 1;
    if (need > cache_cap) {
        cache = seq_realloc(cache, cache_cap, need);
        cache_cap = need;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)26;
        else if (i == 2)
            cache[2] = (seq_t)431;
        else if (i == 3)
            cache[3] = (seq_t)5824;
        else {
            /* compute: a(n)=26*a(n-1)+988*a(n-3) - (245*a(n-2)+1440*a(n-4)) */
            seq_t part1 = cache[i-1] * (seq_t)26;
            seq_t part2 = cache[i-3] * (seq_t)988;
            seq_t sum1  = part1 + part2;          /* positive terms */

            seq_t part3 = cache[i-2] * (seq_t)245;
            seq_t part4 = cache[i-4] * (seq_t)1440;
            seq_t sum2  = part3 + part4;          /* negative terms */

            cache[i] = sum1 - sum2;
        }
    }

    cache_len = need;
    return cache[n];
}
