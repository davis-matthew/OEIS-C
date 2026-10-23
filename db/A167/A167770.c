/*
 * A167770  a(n) = prime(n)^2 modulo prime(n+1).
 * https://oeis.org/A167770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 modulo prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6926, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Static storage for the prime list (1‑based indexing) */
static seq_t *prime_list = NULL;   /* array of primes */
static idx_t prime_count = 0;      /* number of primes stored       */
static idx_t sieve_limit = 0;      /* largest value that has been sieved */

/* Rebuild the primality sieve up to 'limit' and store all primes found */
static void rebuild_primes_upto(idx_t limit)
{
    /* Allocate a simple boolean sieve */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return;                          /* memory failure – caller will handle */
    memset(is_prime + 2, 1, (size_t)(limit - 1));    /* 2 … limit are prime candidates */

    /* Classic Eratosthenes: cross off composites */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; i++) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes to allocate exactly the space needed */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; i++)
        if (is_prime[i]) cnt++;

    /* Reallocate the prime list to accommodate all primes <= limit */
    seq_t *new_list = (seq_t *)realloc(prime_list, (size_t)cnt * sizeof(seq_t));
    if (!new_list) {
        free(is_prime);
        return;
    }
    prime_list = new_list;

    /* Copy the primes into the list (1‑based indexing) */
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; i++)
        if (is_prime[i]) prime_list[pos++] = (seq_t)i;

    prime_count  = cnt;
    sieve_limit  = limit;

    free(is_prime);
}

/* Ensure that we have at least 'needed' primes (1‑based count) */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= needed) return;

    /* Start with a modest sieve range and double until we have enough primes */
    idx_t limit = (sieve_limit > 0) ? sieve_limit : 2000;
    /* Use Rosser's bound for a better starting estimate          */
    if (needed >= 6) {
        double n = (double)needed;
        double estimate = n * (log(n) + log(log(n))) + 3.0;
        if ((idx_t)estimate > limit) limit = (idx_t)estimate;
    }

    while (prime_count < needed) {
        rebuild_primes_upto(limit);
        /* Increase the limit for the next iteration */
        limit *= 2;
    }
}

/* Compute a(n) = prime(n)^2 modulo prime(n+1)  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 – return safe default */

    /* We need primes for indices up to n+1 (1‑based) */
    ensure_primes(n + 1);
    if (prime_count <= n) return 0; /* fallback if sieve failed */

    seq_t p_n    = prime_list[n - 1]; /* prime(n)  */
    seq_t p_next = prime_list[n];     /* prime(n+1) */

    seq_t sq = p_n * p_n;             /* fits in seq_t for n <= 10000   */
    return sq % p_next;
}
