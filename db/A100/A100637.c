/*
 * A100637  Trisection of A000720.
 * https://oeis.org/A100637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes <= 3*n + 3 (prime counting function)
 * Range:     u8 n <= 538, u16 n <= 9075, u32 n <= 9075, u64 n <= 9075,
 *            u128 n <= 9075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for prime sieve and cumulative prime counts. */
static bool *prime = NULL;
static seq_t *prime_pi = NULL;
static idx_t sieve_limit = 0;

/* Helper: extend sieve up to new_limit (inclusive). */
static void ensure_sieve(idx_t new_limit) {
    if (sieve_limit >= new_limit)
        return;

    /* Allocate new arrays for the larger limit. */
    bool *new_prime = (bool *)malloc((size_t)(new_limit + 1) * sizeof(bool));
    seq_t *new_pi = (seq_t *)malloc((size_t)(new_limit + 1) * sizeof(seq_t));
    if (!new_prime || !new_pi) {
        /* Allocation failed: leave state unchanged. */
        return;
    }

    /* Initialize all entries as prime candidates. */
    for (idx_t i = 0; i <= new_limit; ++i)
        new_prime[i] = true;
    new_prime[0] = new_prime[1] = false;

    /* Sieve of Eratosthenes. */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= new_limit)
        ++r;
    for (idx_t p = 2; p <= r; ++p) {
        if (new_prime[p]) {
            for (idx_t j = p * p; j <= new_limit; j += p)
                new_prime[j] = false;
        }
    }

    /* Build cumulative prime counts. */
    seq_t count = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (new_prime[i])
            ++count;
        new_pi[i] = count;
    }

    /* Release old arrays. */
    free(prime);
    free(prime_pi);

    /* Install new arrays. */
    prime = new_prime;
    prime_pi = new_pi;
    sieve_limit = new_limit;
}

/* Public sequence generator. */
seq_t a(idx_t n) {
    idx_t m = 3 * n + 3;          /* The argument to the prime counting function. */
    ensure_sieve(m);
    return prime_pi[m];
}
