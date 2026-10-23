/*
 * A037166  a(n) = prime(n)*prime(n+1) - prime(n).
 * https://oeis.org/A037166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * prime(n+1) - prime(n) (offset = 1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of prime numbers (uint64_t works for all seq_t).  */
static uint64_t *prime_cache = NULL;   /* pointer to allocated array      */
static size_t  prime_count = 0;        /* number of stored primes          */
static size_t  prime_cap   = 0;        /* current capacity of the array    */

/* Ensure that prime_cache contains at least (needed + 1) primes
   (indices 0..needed).  Needed is given as idx_t to match the
   external interface.  */
static void
ensure_primes(idx_t needed)
{
    /* Grow the cache if it hasn't been allocated yet.  */
    if (prime_cache == NULL) {
        prime_cap   = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least (needed + 1) of them. */
    while (prime_count <= (size_t)needed) {
        uint64_t candidate = (prime_count == 0) ? 2 : prime_cache[prime_count - 1] + 1;
        /* Find the next prime by trial division using primes already found. */
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t p = prime_cache[i];
                if (p * p > candidate) {
                    break;          /* no need to test larger primes */
                }
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                break;              /* candidate is a prime */
            }
            ++candidate;
        }

        /* Grow the storage if necessary. */
        if (prime_count == prime_cap) {
            prime_cap <<= 1;        /* double the capacity */
            uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                prime_cap * sizeof(uint64_t));
            prime_cache = tmp;
        }

        /* Store the newly found prime. */
        prime_cache[prime_count++] = candidate;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)           /* offset is 1, guard against 0 */
        return 0;

    /* prime(n) is at index n‑1, prime(n+1) at index n. */
    ensure_primes(n);     /* make sure we have primes[0..n] */

    seq_t p1 = (seq_t)prime_cache[n - 1];
    seq_t p2 = (seq_t)prime_cache[n];

    return p1 * p2 - p1;
}
