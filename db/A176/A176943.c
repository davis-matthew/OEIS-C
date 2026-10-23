/*
 * A176943  Primes of the form 43^k+10^k.
 * https://oeis.org/A176943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   find primes of the form 43^k + 10^k for k >= 0. a(1) = 2, a(2) =
 *            53,
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle the offset: a(1) corresponds to k = 0. */
    if (n == 0) return 0;

    /* Static cache of previously found primes. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;

    /* Current value of 43^k and 10^k.  Initialise for k = 0. */
    static seq_t cur43 = 1;
    static seq_t cur10 = 1;

    /* Grow the cache when needed. */
    if (cache_cnt < n)
    {
        /* Compute primes until we have at least n of them. */
        while (cache_cnt < n)
        {
            seq_t candidate = cur43 + cur10;
            if (is_prime_big(candidate))
            {
                /* Ensure capacity. */
                if (cache_cnt == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_cnt++] = candidate;
            }

            /* Prepare next k: 43^(k+1) and 10^(k+1). */
            cur43 = cur43 * 43;
            cur10 = cur10 * 10;
        }
    }

    return cache[n - 1];
}
