/*
 * A132230  Primes congruent to 1 (mod 30).
 * https://oeis.org/A132230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 1 (mod 30)
 * Range:     u8 n <= 6, u16 n <= 809, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of found primes (stored as 64‑bit values).  The sequence values
   themselves are returned as seq_t, which may be 8/16/32/64/128 bits or an
   arbitrary–precision integer class. */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t  cache_len  = 0;          /* number of primes stored */
static size_t  cache_cap  = 0;          /* allocated capacity */

/* Sieve state ----------------------------------------------------------- */
static uint64_t sieve_limit = 0;   /* current limit of the sieve */
static uint64_t last_limit   = 0;   /* previous upper bound of the sieve */

/* Resize the prime cache when more space is required.  */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *new_arr = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!new_arr) {
        /* Allocation failed – program will abort on subsequent call.  */
        exit(EXIT_FAILURE);
    }
    prime_cache = new_arr;
    cache_cap   = new_cap;
}

/* Extend the sieve until the cache contains at least n primes.  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len)
        return;

    /* Initialise the sieve limit on the first call.  */
    if (sieve_limit == 0)
        sieve_limit = 200;

    while ((size_t)n > cache_len) {
        /* Grow the sieve range.  */
        if (sieve_limit < 200)
            sieve_limit = 200;
        else
            sieve_limit *= 2;

        /* Allocate array for the sieve.  */
        uint8_t *is_prime = (uint8_t *)calloc(sieve_limit + 1, sizeof(uint8_t));
        if (!is_prime) {
            exit(EXIT_FAILURE);
        }

        /* Default: mark all as potential primes.  */
        for (uint64_t i = 2; i <= sieve_limit; ++i)
            is_prime[i] = 1;

        /* Sieve of Eratosthenes.  */
        for (uint64_t i = 2; i * i <= sieve_limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= sieve_limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Append new primes ≡ 1 (mod 30).  */
        for (uint64_t num = last_limit + 1; num <= sieve_limit; ++num) {
            if (num >= 2 && is_prime[num] && (num % 30ULL == 1ULL)) {
                ensure_cache_capacity(cache_len + 1);
                prime_cache[cache_len++] = num;
            }
        }

        free(is_prime);
        last_limit = sieve_limit;
    }
}

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n <= 0, but harmless */
    ensure_cache(n);
    return (seq_t)prime_cache[n - 1];
}
