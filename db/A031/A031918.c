/*
 * A031918  a(n) = prime(9*n-8).
 * https://oeis.org/A031918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(9*n-8)
 * Range:     u8 n <= 6, u16 n <= 727, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The sequence is defined as the (9*n-8)-th prime.
   We generate primes incrementally and cache them. */

static seq_t *prime_cache = NULL;   /* 0‑based array of primes */
static idx_t  prime_count = 0;      /* current number of cached primes */
static idx_t  next_candidate = 2;   /* next integer to test for primality */

/* Ensure that prime_cache contains at least `k` primes.
   k >= 1.  The function is idempotent and thread‑unsafe. */
static void
ensure_k_primes(idx_t k)
{
    if (prime_count >= k)
        return;

    /* We grow the cache as needed. */
    while (prime_count < k) {
        /* Test whether next_candidate is prime, using only the primes found
           so far for trial division. The loop stops when the smallest
           square divisor is greater than the candidate. */
        bool is_prime = true;
        for (idx_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = (uint64_t)prime_cache[pi];
            if (p * p > (uint64_t)next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Append this new prime to the cache. */
            seq_t *tmp = (seq_t *)realloc(prime_cache,
                                          (prime_count + 1) * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            prime_cache = tmp;
            prime_cache[prime_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0, return 0 */

    idx_t k = 9 * n - 8;          /* target prime index (1‑based) */
    ensure_k_primes(k);
    return prime_cache[k - 1];    /* cache is 0‑based */
}
