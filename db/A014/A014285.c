/*
 * A014285  a(n) = Sum_{j=1..n} j*prime(j).
 * https://oeis.org/A014285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..n} j*prime(j)
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute the nth sum of j times the j‑th prime.
   The function may be called in any order and is fully
   self‑contained, using only standard headers that are
   accepted in both C11 and C++17. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n < 1)
        return 0;

    /* Rough upper bound for the n-th prime (works for n <= 10000
       and is conservative for larger n). */
    idx_t limit = n * 20ULL + 100;
    if (limit < 200)
        limit = 200;

    /* Allocate the sieve array.  In C++ a cast is required for
       the result of calloc, so we cast explicitly. */
    size_t sieve_size = (size_t)limit + 1;
    char *is_not_prime = (char*)calloc(sieve_size, 1);
    if (!is_not_prime)
        return 0;  /* Allocation failed – return 0 as safe value */

    /* Allocate storage for the first n primes. */
    idx_t *primes = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!primes) {
        free(is_not_prime);
        return 0;  /* Allocation failed */
    }

    /* Simple Sieve of Eratosthenes.  We stop after collecting
       the first n primes, but we still need the full sieve
       to correctly mark composites. */
    idx_t prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_not_prime[i] != 0)
            continue;  /* composite */

        /* i is prime */
        if (prime_count < n)
            primes[prime_count++] = i;

        /* Mark multiples of i.  Guard against overflow. */
        if ((uint64_t)i * i <= limit) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_not_prime[j] = 1;
        }
    }

    /* Compute the required sum: Σ j * prime(j). */
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        sum += (seq_t)j * (seq_t)primes[j - 1];
    }

    free(is_not_prime);
    free(primes);
    return sum;
}
