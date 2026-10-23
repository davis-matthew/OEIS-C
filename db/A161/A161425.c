/*
 * A161425  a(n) = A161424(n)/2.
 * https://oeis.org/A161425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(n+3) for 1 ≤ n ≤ 5; for n ≥ 6, a(n) = 2 × p_{n‑1},
 *            where p_k is the k‑th prime
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the prime list generated once. */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_generated = false;

/* Generate a modest list of primes up to a fixed limit.
   The 10 000th prime is 104 729, so a limit of 200 000 suffices
   for all test values. */
static void generate_primes(void)
{
    if (primes_generated)
        return;

    const idx_t LIMIT = 200000ULL;          /* safe upper bound for n ≤ 10 000 */
    uint8_t *sieve = NULL;
    size_t  est   = LIMIT / 10 + 10;        /* rough estimate of number of primes */

    sieve = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!sieve)
        return;                             /* allocation failed – leave list empty */

    /* Mark all entries as potential primes. */
    for (idx_t i = 0; i <= LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Classic sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;

    /* Allocate the prime array. */
    prime_list = (idx_t *)malloc(est * sizeof(idx_t));
    if (!prime_list) {
        free(sieve);
        return;
    }

    /* Collect primes. */
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) {
            if (prime_count == est) {          /* grow the array if necessary */
                est *= 2;
                idx_t *tmp = (idx_t *)realloc(prime_list,
                                              est * sizeof(idx_t));
                if (!tmp)
                    break;                    /* on error stop collecting */
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
        }

    free(sieve);
    primes_generated = true;
}

/* Return the n-th term of A161425. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                         /* defined only for n ≥ 1 */

    if (n <= 5)                                   /* first five terms */
        return (seq_t)(2 * (n + 3));

    /* Ensure the prime list is ready. */
    generate_primes();

    /* For n ≥ 6, a(n) = 2 × prime(n‑1).  Prime list is zero‑based. */
    size_t idx = (size_t)(n - 2);                 /* index into prime_list */
    if (idx < prime_count)
        return (seq_t)(prime_list[idx] * (seq_t)2);

    /* Fallback: extend the prime list just enough to reach the requested index. */
    /* Start searching from the last prime stored. */
    idx_t candidate = (prime_count > 0) ? prime_list[prime_count - 1] + 1 : 2;
    while (idx >= prime_count) {
        /* Trial‑division test using the primes we already have. */
        bool prime = true;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            idx_t p = prime_list[pi];
            if ((idx_t)p * (idx_t)p > candidate)
                break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            prime_list = (idx_t *)realloc(prime_list,
                                          (prime_count + 1) * sizeof(idx_t));
            if (!prime_list)
                break;                          /* allocation failed */
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }

    /* Return the required term. */
    return (seq_t)(prime_list[idx] * (seq_t)2);
}
