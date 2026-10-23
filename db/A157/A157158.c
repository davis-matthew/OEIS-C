/*
 * A157158  Primes where the first digit is equal to the largest digit.
 * https://oeis.org/A157158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = nth prime where the first decimal digit equals the
 *            largest decimal digit of the prime
 * Range:     u8 n <= 16, u16 n <= 717, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   first_digit_equals_largest
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/first_digit_equals_largest.h"

/* Helper: test whether x is prime using the list of primes discovered
   so far. */
static bool is_prime_candidate(uint64_t x, const uint64_t *primes, size_t primes_size)
{
    for (size_t i = 0; i < primes_size && primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) return false;
    }
    return true;
}

/*  a(n)  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* array of found terms          */
    static size_t cache_size  = 0;         /* number of terms stored       */
    static size_t cache_ready = 0;         /* number of terms computed     */
    static uint64_t *prime_list = NULL;    /* list of all primes found     */
    static size_t primes_size = 0;         /* number of primes stored      */
    static size_t primes_cap  = 0;         /* capacity of prime array      */
    static uint64_t next_candidate = 2;    /* next number to test for prime*/

    /* Grow cache if necessary to hold at least ‘n’ results. */
    if (cache_ready < (size_t)n) {
        /* Ensure cache has capacity for the new values. */
        if (cache_size < (size_t)n) {
            size_t new_cap = cache_size ? cache_size * 2 : 8;
            while (new_cap < (size_t)n) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_size = new_cap;
        }

        /* Grow prime list capacity if needed. */
        if (!prime_list) {
            primes_cap = 8;
            prime_list = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        }

        /* Continue searching until we have ‘n’ terms. */
        while (cache_ready < (size_t)n) {
            uint64_t p = next_candidate;

            /* Check primality. */
            bool is_prime = is_prime_candidate(p, prime_list, primes_size);

            /* Advance candidate for next round. */
            next_candidate += (p == 2) ? 1 : 2;

            if (!is_prime) continue;

            /* Store the new prime. */
            if (primes_size == primes_cap) {
                primes_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, primes_cap * sizeof(uint64_t));
            }
            prime_list[primes_size++] = p;

            /* Does it satisfy the digit condition? */
            if (first_digit_equals_largest(p)) {
                cache[cache_ready++] = (seq_t)p;
            }
        }
    }
    return cache[n - 1];
}
