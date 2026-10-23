/*
 * A021849  Decimal expansion of 1/845.
 * https://oeis.org/A021849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of 1/845 (a(0) is the first digit
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* cached digits */
static idx_t cache_len = 0;    /* number of digits currently cached */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

seq_t a(idx_t n)
{
    /* Expand the cache if necessary */
    if (n >= cache_len)
    {
        /* Allocate more space */
        size_t new_cap = (cache_len == 0 ? 16 : cache_len * 2);
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failed, return 0 as a safe default */
            return 0;
        cache = new_cache;
        cache_cap = new_cap;

        /* Generate missing digits */
        uint64_t rem = 1;        /* remainder of 1/845 */
        /* generate digits up to cache_len-1 first if cache_len==0 */
        for (idx_t i = 0; i < cache_len; ++i)
        {
            uint64_t tmp = rem * 10ULL;
            cache[i] = (seq_t)(tmp / 845ULL);
            rem = tmp % 845ULL;
        }

        /* continue from cache_len to n */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            uint64_t tmp = rem * 10ULL;
            cache[i] = (seq_t)(tmp / 845ULL);
            rem = tmp % 845ULL;
        }

        cache_len = n + 1;
    }

    return cache[n];
}
