/*
 * A097452  Primes of the form prime(k) + nonprime(k) for some k.
 * https://oeis.org/A097452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 705, u32 n <= 421, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A097452  – primes of the form prime(k) + nonprime(k) for some k
   Formula:  a(n) = prime(k) + nonprime(k)  where k is the index of the term
   (k = a(n) index).  The sequence is defined for n >= 1.  The implementation
   builds the sequence incrementally, caching results so that multiple calls
   with arbitrary n are served quickly. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, but guard against n == 0 */

    /* static state shared by all calls */
    static seq_t *cache      = NULL;   /* array of found primes in the sequence */
    static size_t cache_cap  = 0;      /* current capacity of the cache array   */
    static size_t cache_len  = 0;      /* number of elements currently stored   */

    static idx_t next_k          = 0;   /* k value for the next candidate        */
    static idx_t nonprime_index  = 0;   /* count of nonprimes seen so far        */
    static uint64_t cur_nonprime = 0;   /* current integer being examined       */

    /* grow the cache until we have at least n terms */
    while (cache_len < n)
    {
        /* 1.  Prepare next k */
        ++next_k;

        /* 2.  Get the k-th prime */
        uint32_t pk = get_prime(next_k);

        /* 3.  Find the k-th nonprime.  The list of nonprimes starts at 1. */
        while (nonprime_index < next_k)
        {
            ++cur_nonprime;
            if (!is_prime_big((seq_t)cur_nonprime))
                ++nonprime_index;
        }

        /* 4.  Compute the candidate sum */
        seq_t candidate = (seq_t)pk + (seq_t)cur_nonprime;

        /* 5.  If the sum is prime, add it to the cache */
        if (is_prime_big(candidate))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – unlikely under normal use */
                    return candidate;
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
    }

    /* 6.  Return the requested term */
    return cache[n - 1];
}
