/*
 * A171021  In the sequence of prime numbers, replace all the '4' digits with
 *          '2' and vice versa.
 * https://oeis.org/A171021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   swap_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171021: swap 2<->4 in primes (in decimal representation) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/swap_digits.h"

/* Helpers to build the prime table ----------------------------------------------------- */
static uint64_t *prime_cache      = NULL;   /* dynamic array of primes (uint64)          */
static size_t   prime_count       = 0;      /* number of primes stored                   */
static size_t   prime_capacity    = 0;      /* current allocated size                   */
static uint64_t next_candidate    = 3;      /* next odd number to be tested for prime    */

/* Ensure that `prime_cache` contains at least `n` primes (1‑based). */
static void ensure_primes(idx_t n)
{
    /* Grow the array if needed. */
    if (prime_capacity < prime_count) {
        prime_capacity = (prime_capacity == 0) ? 16 : prime_capacity * 2;
        prime_cache = (uint64_t *)realloc(prime_cache, prime_capacity * sizeof(uint64_t));
    }

    /* Initialise with the first prime if the cache is empty. */
    if (prime_count == 0) {
        if (prime_capacity == 0)
            prime_cache = (uint64_t *)malloc(16 * sizeof(uint64_t));
        prime_cache[0] = 2;
        prime_count = 1;
        next_candidate = 3;
    }

    /* Generate further primes until we have at least `n`. */
    while (prime_count < n) {
        uint64_t p = next_candidate;
        int is_prime = 1;
        /* Trial division using primes already found. */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_cache[i];
            if (q * q > p)
                break;
            if (p % q == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_capacity == prime_count) {
                prime_capacity = (prime_capacity == 0) ? 16 : prime_capacity * 2;
                prime_cache = (uint64_t *)realloc(prime_cache, prime_capacity * sizeof(uint64_t));
            }
            prime_cache[prime_count++] = p;
        }
        next_candidate += 2;          /* only odd numbers need testing             */
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* the problem does not impose any constraints on the order
       of calls, therefore caching primes is appropriate. */
    ensure_primes(n);

    /* the nth prime, 1‑based indexing */
    seq_t p = (seq_t)prime_cache[n - 1];

    /* swap 2↔4 digits */
    return swap_digits(p);
}
