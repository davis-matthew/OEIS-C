/*
 * A163422  Primes p such that A071568((p-1)/2) is also prime.
 * https://oeis.org/A163422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 142, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163422:
   a(n) = the nth prime p such that ((p-1)/2)^3 + ((p-1)/2) + 1 is prime.
   (For odd prime p, the expression equals m^3 + m + 1 with m = (p-1)/2.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */
#include "../utils/is_prime.h"        /* bool is_prime(idx_t n); */

/* static cache of generated terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* prime generation helpers */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_len = 0;
static uint64_t next_candidate = 3;  /* start testing at 3 */

/* Grow the prime list vector when needed */
static void grow_prime_list(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);            /* allocation failure – abort */
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Grow the result cache vector when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based).  Generates more terms if necessary. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0 */

    /* generate terms until we have at least n */
    while (cache_len < (size_t)n) {
        uint64_t p = 0;

        /* Find the next prime */
        for (;;) {
            uint64_t candidate = next_candidate;
            next_candidate += 2;                 /* only odd candidates */

            /* trial division by known primes */
            bool is_p = true;
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t q = prime_list[i];
                if ((unsigned __int128)q * q > candidate) break;
                if (candidate % q == 0) { is_p = false; break; }
            }
            if (is_p) {
                p = candidate;
                /* store in prime list */
                if (prime_len == prime_cap) grow_prime_list();
                prime_list[prime_len++] = p;
                break;
            }
        }

        /* condition: ((p-1)/2)^3 + ((p-1)/2) + 1 is prime */
        uint64_t m = (p - 1) / 2;
        unsigned __int128 tmp = (unsigned __int128)m * m * m + m + 1;
        seq_t val = (seq_t)tmp;          /* safe cast for builds where val fits seq_t */

        if (isPrimeGeneric(val)) {
            /* add to cache */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
