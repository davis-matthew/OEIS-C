/*
 * A203474  a(n) = A203472(n) / A000178(n-1), where A000178 are the
 *          superfactorials.
 * https://oeis.org/A203474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n} binomial(2*j+3, j+4) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 7, a(3) = 252, ... as in OEIS A203474 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* static state for memoisation (any order of calls is supported) */
    static seq_t *cache       = NULL;   /* array of products, index = n */
    static idx_t cached_max   = 0;      /* largest n whose value is cached */
    static idx_t cache_cap    = 0;      /* allocated capacity (≥ cached_max+1) */
    static seq_t prod         = (seq_t)1;/* product of binomials up to cached_max */

    if (n == 0)                     /* sequence is defined for n ≥ 1, return 0 for safety */
        return (seq_t)0;

    /* Allocate cache on first call or grow it if needed */
    if (cache == NULL || n >= cache_cap)
    {
        idx_t new_cap = n + 1;
        if (new_cap <= cache_cap) new_cap = cache_cap * 2;  /* grow by factor 2 if not large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                        /* allocation failed – give up */
            return prod;

        cache  = tmp;
        cache_cap = new_cap;

        /* initialise new entries to 0 (not strictly necessary but safe) */
        for (idx_t i = cached_max + 1; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Extend cache up to n if not already cached */
    if (n > cached_max)
    {
        for (idx_t j = cached_max + 1; j <= n; ++j)
        {
            seq_t bin = binomial((idx_t)(2 * j + 3), (idx_t)(j + 4));
            prod = prod * bin;          /* seq_t multiplication is allowed */
            cache[j] = prod;
        }
        cached_max = n;
    }

    return cache[n];
}
