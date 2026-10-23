/*
 * A145483  Primes p such that 2*p - 23 is prime.
 * https://oeis.org/A145483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 891, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n-th prime p such that 2*p-23 is also prime (OEIS A145483) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Internal cache of the sequence */
static seq_t *cache = NULL;   /* array of found primes */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static size_t cache_len = 0;  /* number of terms currently stored */
static idx_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure that the cache has at least <min_cap> slots. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    /* realloc must be cast in C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* fatal allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Find the nth term (n >= 1).  Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe fallback */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Advance to the next prime. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Check the condition 2*p - 23 prime. */
        idx_t check = 2 * next_candidate - 23;
        if (check > 1 && is_prime(check))
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;   /* move on for the next search */
    }

    return cache[n - 1];
}
