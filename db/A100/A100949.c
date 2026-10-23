/*
 * A100949  Number of partitions of n into a prime and a semiprime.
 * https://oeis.org/A100949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered pairs {p,s} with p prime, s semiprime
 *            and p + s = n (OEIS A100949)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for the helper functions and types */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static state: prime and semiprime tables, and the largest n
   for which they are valid.  The arrays are allocated in
   recompute() and freed before reallocation. */
static idx_t max_n = 0;
static bool *prime = NULL;      /* prime[i] == true iff i is prime          */
static bool *semiprime = NULL;  /* semiprime[i] == true iff i = p*q with
                                  p,q primes.                           */

/* Recompute prime and semiprime tables up to the given limit.  The
   function is only called when a new value of n is larger than the
   previously computed maximum. */
static void recompute(idx_t limit)
{
    /* 1. Sieve of Eratosthenes: compute primality of all numbers
       up to 'limit'. */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return; /* out of memory: keep the old tables */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;

    /* 2. Store the new prime table.  Allocate fresh memory
       and copy the sieve results. */
    bool *new_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!new_prime) {
        free(is_prime);
        return; /* memory error: keep the old tables */
    }
    for (idx_t i = 0; i <= limit; ++i) new_prime[i] = is_prime[i];

    /* 3. Allocate and compute the semiprime table. */
    bool *new_semiprime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!new_semiprime) {
        free(is_prime);
        free(new_prime);
        return; /* memory error: keep the old tables */
    }
    for (idx_t i = 0; i <= limit; ++i) new_semiprime[i] = false;

    for (idx_t p = 2; p <= limit; ++p) if (new_prime[p]) {
        for (idx_t q = p; q <= limit / p; ++q) if (new_prime[q]) {
            idx_t prod = p * q;
            if (prod <= limit) new_semiprime[prod] = true;
        }
    }

    /* 4. Replace the old tables with the new ones. */
    free(is_prime);
    free(prime);
    free(semiprime);
    prime     = new_prime;
    semiprime = new_semiprime;
    max_n     = limit;
}

/* ------------------------------------------------------------------ */
/* Function required by the build environment.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safe guard – the specification
                                     only requires n ≥ 1, but this
                                     makes the function robust.     */

    /* Make sure tables are available up to n. */
    if (n > max_n) recompute(n);

    seq_t total = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        idx_t j = n - i;
        if (prime[i] && semiprime[j])  ++total;
        if (semiprime[i] && prime[j]) ++total;
    }
    return total;
}
