/*
 * A239619  Base 3 sum of digits of prime(n).
 * https://oeis.org/A239619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of base‑3 digits of the n-th prime (OEIS A239619)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h" /* provides is_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* out of spec but safe */

    /* static cache of found primes for repeated calls */
    static uint64_t *prime_list   = NULL; /* array of primes */
    static size_t   primes_used   = 0;    /* number of primes stored */
    static size_t   prime_cap     = 0;    /* allocated capacity */
    static uint64_t candidate     = 2;    /* current integer to test for primality */

    /* grow the prime array if necessary */
    if (prime_cap == 0) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* find primes until we have at least n primes */
    while (primes_used < (size_t)n) {
        while (!is_prime(candidate))
            ++candidate;

        /* store the found prime */
        if (primes_used >= prime_cap) {
            prime_cap *= 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (!tmp) {
                free(prime_list);
                return 0; /* allocation failure: return zero as safe fallback */
            }
            prime_list = tmp;
        }
        prime_list[primes_used++] = candidate;

        ++candidate;
    }

    /* compute the sum of base‑3 digits of the n-th prime */
    uint64_t p   = prime_list[n - 1];
    seq_t    sum = (seq_t)0;
    while (p) {
        seq_t digit = (seq_t)(p % 3ULL);
        sum += digit;
        p /= 3ULL;
    }
    return sum;
}
