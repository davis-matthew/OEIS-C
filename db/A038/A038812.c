/*
 * A038812  Number of primes less than 1000n.
 * https://oeis.org/A038812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes < 1000 n (equivalently a(n) = pi(1000 n
 *            – 1)
 * Range:     u8 n <= 1, u16 n <= 821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for the sieve and prefix‐sum array */
static size_t  current_limit = 0;          /* highest index that has been sieved */
static bool   *is_prime      = NULL;      /* is_prime[i] == true iff i is prime      */
static uint32_t *pi_prefix   = NULL;      /* pi_prefix[i] == #primes ≤ i              */
static size_t   pi_cap       = 0;          /* allocated size of pi_prefix              */

seq_t a(idx_t n)
{
    /* limit = 1000 * n    (primes < limit)            */
    if (n == 0) return (seq_t)0;            /* out‑of‑range not expected but safe    */
    size_t limit = (size_t)((seq_t)n * 1000); /* convert to size_t for array indices */

    /* grow the sieve if needed */
    if (limit > current_limit) {
        /* reallocate arrays */
        is_prime = (bool *)realloc(is_prime, limit + 1);
        if (!is_prime) exit(1);                 /* allocation failed */
        pi_prefix = (uint32_t *)realloc(pi_prefix, (limit + 1) * sizeof(uint32_t));
        if (!pi_prefix) exit(1);
        pi_cap = limit + 1;

        /* initialization: mark all as potential primes */
        for (size_t i = 0; i <= limit; ++i)
            is_prime[i] = (i >= 2);

        /* classic Eratosthenes sieve */
        for (size_t p = 2; p * p <= limit; ++p) {
            if (!is_prime[p]) continue;
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = false;
        }

        /* build prefix sums of prime counts */
        pi_prefix[0] = 0;
        for (size_t i = 1; i <= limit; ++i)
            pi_prefix[i] = pi_prefix[i - 1] + (is_prime[i] ? 1 : 0);

        current_limit = limit;
    }

    /* number of primes strictly less than limit = primes ≤ limit‑1   */
    if (limit == 0) return (seq_t)0;          /* safety for n==0                */
    return (seq_t)pi_prefix[limit - 1];
}
