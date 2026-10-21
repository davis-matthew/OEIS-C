/*
 * A095280  Lower Wythoff primes, i.e., primes in A000201.
 * https://oeis.org/A095280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 4078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A095280 - Lower Wythoff primes
   Formula: a(n) = n-th prime appearing in the lower Wythoff sequence
            a_k = floor(k*φ)   (φ = (1+√5)/2).
   The function enumerates floor(k*φ) for k = 1,2,…, testing each
   value for primality, and returns the n‑th such prime.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"    /* is_prime(idx_t)   */

seq_t a(idx_t n)
{
    /* golden ratio; long double for extra precision */
    const long double phi = 1.6180339887498948482045868343656381177203L;

    /* static cache for discovered primes of the sequence */
    static seq_t *prime_cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static counter for k in floor(k*φ) */
    static idx_t k_next = 1;

    /* static sieve to test numbers up to a generous bound */
    static bool *sieve = NULL;
    static size_t sieve_limit = 0;

    /* Build the sieve once: enough for n up to ~10 000   */
    if (!sieve) {
        sieve_limit = 20000000U;                 /* 20 million */
        sieve = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
        if (!sieve) return 0U;                  /* allocation failure */

        for (size_t i = 0; i <= sieve_limit; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;

        for (size_t i = 2; i * i <= sieve_limit; ++i) {
            if (sieve[i]) {
                for (size_t j = i * i; j <= sieve_limit; j += i)
                    sieve[j] = false;
            }
        }
    }

    /* Grow the cache until it contains at least n primes */
    while (cache_len < n) {
        /* floor(k*φ) using long double then cast to uint64_t */
        uint64_t val_u64 = (uint64_t)((long double)k_next * phi);
        seq_t val = val_u64;                     /* constructor from uint64_t */

        bool isprime = false;
        if (val_u64 <= sieve_limit) {
            isprime = sieve[(size_t)val_u64];
        } else {
            /* fallback: primality test for larger numbers */
            isprime = is_prime(val_u64);
        }

        if (isprime) {
            if (cache_len >= cache_cap) {        /* grow cache if needed */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) return 0U;            /* allocation failure */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = val;
        }
        ++k_next;
    }

    return prime_cache[n - 1];
}
