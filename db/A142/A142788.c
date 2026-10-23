/*
 * A142788  Primes congruent to 17 mod 60.
 * https://oeis.org/A142788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 410, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_generic, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 17 (mod 60)
 * Algorithm: iterate over increasing integers, test primality and
 * the congruence condition.  The sequence is memoised so any order of
 * calls works.  The iterable search uses the supplied next_prime()
 * routine, which returns the first prime > given number; by
 * calling it with `candidate-1` we include the current candidate. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_generic.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n<1 */

    /* static cache for memoisation */
    static seq_t *cache      = NULL;             /* store found primes */
    static idx_t  cache_sz   = 0;                /* number cached */
    static size_t cache_cap  = 0;                /* allocated capacity */
    static uint64_t start    = 17;               /* next candidate to check */

    /* Grow cache until we have at least n primes */
    while (cache_sz < n)
    {
        uint64_t p;

        /* Use next_prime(start-1) to include 'start' itself when it is a
           prime (for the initial value 17).  For subsequent iterations
           start is always > 17, so this works safely. */
        p = next_prime(start - 1ULL);

        /* If p satisfies congruence, store it */
        if ((p % 60ULL) == 17ULL)
        {
            /* Expand cache if necessary */
            if (cache_sz >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)p;
        }

        /* Prepare next search start: after the found prime */
        start = p + 1ULL;
    }

    return cache[n - 1];
}
