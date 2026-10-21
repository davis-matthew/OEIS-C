/*
 * A091365  Primes p such that the sum of the digits of p is not prime, but
 *          the sum of the cubes of the digits of p is prime.
 * https://oeis.org/A091365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A091365:
   a(n) is the n‑th prime p such that
        sum of the decimal digits of p is not prime
     AND the sum of the cubes of the decimal digits of p is prime.
   The function uses the helper is_prime(idx_t) for all primality tests
   (idx_t is guaranteed to accommodate the tested values).  A static
   cache stores all terms found so far, so repeated calls return
   results in O(1) after the term has been computed once. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;

    /* The next integer to test for primality. */
    static uint64_t next_candidate = 2;

    /* Return the cached value if already computed. */
    if (n <= cached_len)
        return cache[n - 1];

    /* Initialise cache on first use. */
    if (cached_len == 0) {
        cached_cap = 64;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – return 0 */
    }

    /* Pre‑compute 0³ … 9³ once. */
    static const uint64_t cube[10] = {
        0ULL, 1ULL, 8ULL, 27ULL, 64ULL, 125ULL,
        216ULL, 343ULL, 512ULL, 729ULL
    };

    /* Search for additional qualifying primes until we have n terms. */
    while (cached_len < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Compute digit sum and cube digit sum of next_candidate. */
        uint64_t temp = next_candidate;
        uint64_t digit_sum = 0;
        uint64_t cube_sum  = 0;
        while (temp > 0) {
            uint64_t d = temp % 10;
            digit_sum += d;
            cube_sum  += cube[d];
            temp /= 10;
        }

        /* Apply the two required conditions. */
        if (!is_prime(digit_sum) && is_prime(cube_sum)) {
            /* Ensure capacity for the new term. */
            if (cached_len == cached_cap) {
                cached_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return 0; }
                cache = tmp;
            }
            cache[cached_len++] = (seq_t)next_candidate;
        }

        /* Advance to the next integer for the next iteration. */
        ++next_candidate;
    }

    return cache[n - 1];
}
