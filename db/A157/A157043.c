/*
 * A157043  Primes p such that 6p-7, 6p-5, 6p-3, 6p-1 are not prime.
 * https://oeis.org/A157043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 337, u32 n <= 551, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 6p-7, 6p-5, 6p-3, 6p-1 are all composite.
   a(n) is the n‑th such prime (offset 1).  The function builds a
   static cache of found terms, so any order of calls works.   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0; /* number of terms stored */
    static idx_t  cache_cap  = 0; /* allocated capacity     */

    /* next candidate to test for primality */
    static seq_t candidate = 3; /* first odd prime */

    /* initialise cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* find next prime */
        while (!is_prime_big(candidate))
            candidate += 2;          /* skip even numbers */

        seq_t p = candidate;          /* p is prime */

        /* advance candidate for next search */
        candidate += 2;

        /* check the four numbers that must be composite */
        seq_t v1 = p * 6 - 7;
        seq_t v2 = p * 6 - 5;
        seq_t v3 = p * 6 - 3;
        seq_t v4 = p * 6 - 1;

        if (!is_prime_big(v1) && !is_prime_big(v2) &&
            !is_prime_big(v3) && !is_prime_big(v4)) {
            /* ensure enough capacity */
            if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_size++] = p;
        }
    }

    return cache[n - 1];
}
