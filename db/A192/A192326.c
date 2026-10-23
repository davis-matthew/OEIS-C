/*
 * A192326  Remainders of primes divided by odd numbers.
 * https://oeis.org/A192326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod (2n-1)
 * Range:     u8 n <= 54, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Shared cache of primes used by all calls to a. */
static uint64_t *prime_cache = NULL;   /* array of found primes           */
static size_t prime_count = 0;         /* number of primes stored         */
static size_t prime_cap   = 0;         /* current capacity of prime_cache */

/* Ensure that at least 'needed' primes are stored in prime_cache. */
static void ensure_primes(idx_t needed)
{
    /* No work needed if we already have enough primes. */
    if (prime_count >= (size_t)needed)
        return;

    /* Create or enlarge the prime array. */
    if (prime_cap == 0) {
        prime_cap   = 128;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_count >= prime_cap) {
        prime_cap   *= 2;
        prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
    }

    /* Start searching from the last prime already known. */
    uint64_t candidate = (prime_count == 0) ? 2 : prime_cache[prime_count - 1] + 1;

    while (prime_count < (size_t)needed) {
        /* Skip even numbers after 2. */
        if (candidate > 2 && candidate % 2 == 0) {
            candidate++;
            continue;
        }

        /* Test candidate for primality against all stored primes
           up to sqrt(candidate). The check stops if p*p > candidate. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_cache[i] * prime_cache[i] <= candidate; ++i) {
            if (candidate % prime_cache[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count >= prime_cap) {   /* grow if necessary */
                prime_cap *= 2;
                prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
            }
            prime_cache[prime_count++] = candidate;
        }

        candidate++;
    }
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but guard against 0 */
        return 0;

    /* Compute or extend the prime cache up to the nth prime. */
    ensure_primes(n);

    /* Retrieve the nth prime (primes are 1‑based). */
    seq_t prime_val = (seq_t)prime_cache[n - 1];

    /* Compute the odd divisor (2n-1). */
    seq_t divisor = (seq_t)n * 2 - 1;

    /* Return the remainder. */
    return prime_val % divisor;
}
