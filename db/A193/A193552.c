/*
 * A193552  Prime numbers ending in James Bond number ''007''.
 * https://oeis.org/A193552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th prime of the form 1000k + 7 (k≥1)
 * Range:     u8 none, u16 n <= 10, u32 n <= 12, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* E.g. a(1)=4007, a(2)=6007, a(3)=9007, … */

#include <stdlib.h>          /* for realloc */
#include <stdbool.h>         /* for bool */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)          /* undefined for n<1, return 0 as safe value */
        return (seq_t)0;

    /* Static cache of found primes. */
    static seq_t *cache   = NULL;   /* array of primes */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_sz  = 0;    /* number of primes stored */
    static idx_t  next_k   = (idx_t)1;  /* next k to test (candidate = 1000*k+7) */

    /* Expand cache until we have at least n primes. */
    while (cache_sz < (size_t)n)
    {
        idx_t cand_idx = 1000 * next_k + 7;      /* candidate in idx_t */
        seq_t cand     = (seq_t)cand_idx;        /* cast to seq_t for primality */

        if (is_prime_big(cand))
        {
            /* Grow cache if needed */
            if (cache_sz >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)            /* allocation failed: abort with zero */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = cand;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
