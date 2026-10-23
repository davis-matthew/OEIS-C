/*
 * A045707  Primes with first digit 1.
 * https://oeis.org/A045707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal representation starts with 1
 * Range:     u8 n <= 25, u16 n <= 1193, u32 n <= 995, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of primes with leading digit 1, and a cursor for the next
       candidate integer to test.  The cache persists across calls for
       efficient random access. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;      /* number of primes currently stored */
    static size_t cache_cap = 0;     /* allocated capacity of cache pointer */
    static seq_t next = 2;           /* next integer to test for primality */

    /* We need at least n primes in the cache.  Search for new primes
       until we accumulate enough. */
    while (cache_sz < (size_t)n)
    {
        /* Find the next prime number. */
        while (!is_prime_big(next))
            ++next;

        /* next is prime.  Check whether its first decimal digit is 1. */
        seq_t temp = next;
        while (temp >= 10)
            temp /= 10;

        if (temp == 1)
        {
            /* Ensure the cache has enough room. */
            if (cache_sz == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache,
                    new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the prime. */
            cache[cache_sz++] = next;
        }

        /* Move on to the next candidate. */
        ++next;
    }

    return cache[n - 1];
}
