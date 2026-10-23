/*
 * A120639  Primes such that their quadruple is 1 away from a prime number.
 * https://oeis.org/A120639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120639: primes p such that 4*p ± 1 is prime. */
/* The function searches primes incrementally, storing found terms in a static cache. */

/* Include the helpers that allow fast prime testing. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache for memoization. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_search = 2;                 /* next integer to check for primality */

/* Ensure the cache has room for at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
    cache = tmp;
    cache_cap = new_cap;
}

/* Find the next prime ≥ start using the provided is_prime helper. */
static idx_t find_next_prime(idx_t start)
{
    for (idx_t x = start; ; ++x)
        if (is_prime(x))
            return x;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                             /* undefined for n < 1 */

    /* Grow the cache until it contains at least n elements. */
    while (cache_len < n) {
        idx_t p = find_next_prime(next_search);
        idx_t fourp = 4 * p;

        /* Check if 4p-1 or 4p+1 is prime. */
        if (is_prime(fourp - 1) || is_prime(fourp + 1)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)p;     /* store qualifying prime */
        }

        next_search = p + 1;                   /* next search starts after p */
    }

    return cache[n - 1];
}
