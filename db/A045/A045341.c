/*
 * A045341  Primes congruent to {2, 3, 6} mod 7.
 * https://oeis.org/A045341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p mod 7 ∈ {2,3,6}
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static state for memoisation */
static seq_t *cache = NULL;   /* array of found primes (in seq_t) */
static size_t cache_len = 0;  /* number of primes stored */
static idx_t candidate = 1;   /* last tested integer, start before 2 */

/* return the n-th prime p with p mod 7 in {2,3,6} */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* offset is 1, safety */

    /* grow cache until we have at least n elements */
    while (cache_len < n)
    {
        /* find next prime with the required residue */
        while (1)
        {
            candidate++;                            /* move to next integer */

            /* 2 is the only even prime */
            if (candidate == 2)
                break;

            /* skip even numbers */
            if ((candidate & 1) == 0)
                continue;

            /* primality test */
            if (!is_prime(candidate))
                continue;

            /* check residue */
            idx_t r = candidate % 7;
            if (r == 2 || r == 3 || r == 6)
                break;                              /* found the next prime we need */
        }

        /* ensure cache capacity */
        if (cache == NULL)
        {
            cache = (seq_t *)malloc(16 * sizeof(seq_t));
            cache_len = 0;
        }
        else if (cache_len % 16 == 0)
        {
            cache = (seq_t *)realloc(cache,
                                     (cache_len + 16) * sizeof(seq_t));
        }

        /* store the prime into the cache, cast to seq_t */
        cache[cache_len++] = (seq_t)candidate;
    }

    return cache[n - 1];
}
