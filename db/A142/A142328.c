/*
 * A142328  Primes congruent to 31 mod 45.
 * https://oeis.org/A142328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is congruent to 31 (mod 45)
 * Range:     u8 n <= 2, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache of primes ≡ 31 (mod 45).  The cache holds plain uint64_t values;
   the conversion to the build‑specific seq_t type is performed on demand. */
static uint64_t *prime_cache = NULL;   /* array of cached primes */
static size_t    prime_cap   = 0;       /* capacity of prime_cache          */
static idx_t     prime_size  = 0;       /* number of primes stored          */

/* Sieve workspace */
static idx_t      sieve_limit = 0;      /* current upper bound of sieve    */
static uint8_t   *is_comp = NULL;       /* composite flag array             */
static size_t     comp_cap = 0;         /* size of is_comp                  */

/* Ensure that prime_cache contains at least n primes */
static void ensure_prime_cache(idx_t n)
{
    if (n <= prime_size)
        return;                         /* already have enough */

    /* Initial sieve bound.  1000 is enough to find the first 10 primes. */
    if (sieve_limit == 0)
        sieve_limit = 1000;

    while (prime_size < n) {
        /* Increase the sieve bound geometrically. */
        sieve_limit *= 2;

        /* Allocate (or reallocate) the composite flag array. */
        if (comp_cap < (size_t)(sieve_limit + 1)) {
            size_t new_cap = (size_t)(sieve_limit + 1);
            uint8_t *p = (uint8_t *)realloc(is_comp, new_cap);
            if (!p)
                return;                  /* allocation failed – return */
            is_comp = p;
            comp_cap = new_cap;
        }

        /* Initialise all entries as prime candidates. */
        for (size_t i = 0; i < comp_cap; ++i)
            is_comp[i] = 0;
        is_comp[0] = 1;                     /* 0 is not prime */
        is_comp[1] = 1;                     /* 1 is not prime */

        /* Sieve of Eratosthenes up to sieve_limit. */
        for (idx_t p = 2; p <= sieve_limit / p; ++p) {
            if (is_comp[p])
                continue;
            for (idx_t m = p * p; m <= sieve_limit; m += p)
                is_comp[m] = 1;
        }

        /* Ensure the prime cache has enough room for a second pass. */
        size_t needed = n * 2 + 16;         /* generous over‑estimate */
        if (prime_cap < needed) {
            size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
            if (new_cap < needed)
                new_cap = needed;
            uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                new_cap * sizeof(uint64_t));
            if (!tmp)
                return;                  /* allocation failed – return */
            prime_cache = tmp;
            prime_cap   = new_cap;
        }

        /* Rebuild the list of primes ≡ 31 (mod 45) from scratch. */
        prime_size = 0;
        for (idx_t i = 2; i <= sieve_limit; ++i) {
            if (!is_comp[i] && (i % 45 == 31)) {
                if (prime_size == prime_cap) {   /* safety guard */
                    size_t new_cap = prime_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                        new_cap * sizeof(uint64_t));
                    if (!tmp)
                        return;              /* allocation failed – return */
                    prime_cache = tmp;
                    prime_cap   = new_cap;
                }
                prime_cache[prime_size++] = (uint64_t)i;
            }
        }
        /* If we still don't have enough primes, the while loop continues
           with a larger sieve bound. */
    }
}

/* External sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset starts at 1 */

    ensure_prime_cache(n);
    /* Convert the cached uint64_t to the build‑specific seq_t type. */
    return (seq_t)prime_cache[n - 1];
}
