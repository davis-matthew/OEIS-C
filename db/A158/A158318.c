/*
 * A158318  Primes p such that 5p-2 is prime.
 * https://oeis.org/A158318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p for which 5p-2 is also prime (OEIS
 *            A158318)
 * Range:     u8 n <= 20, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found primes that satisfy the condition */
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;   /* how many terms stored */
    static size_t   cache_cap = 0;   /* allocated capacity */
    static idx_t    next_prime = 3;  /* next odd number to test for primality */

    /* a(0) is undefined; return 0 for safety */
    if (n == 0)
        return 0;

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* memory failure unlikely during tests */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* test current candidate for being prime */
        if (is_prime(next_prime)) {
            idx_t conj = 5 * next_prime - 2;
            if (is_prime(conj)) {
                /* store the good prime */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp)
                        return 0; /* memory failure unlikely */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)next_prime;
            }
        }
        /* next odd number (skipping even numbers) */
        next_prime += 2;
    }

    /* return the requested term (OEIS offset 1) */
    return cache[n - 1];
}
