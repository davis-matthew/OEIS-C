/*
 * A163466  A permutation of two copies of the prime sequence, one moved to
 *          nonprime indices, the other to prime indices.
 * https://oeis.org/A163466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime( ((n is prime) ? pi(n) : (n+1 - pi(n))) )
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where pi(n) is the number of primes ≤ n and
 * prime(k) is the k‑th prime (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- static state for sieve and prime list ---------- */
static unsigned char *is_composite = NULL;   /* 0 → prime candidate, 1 → composite */
static idx_t *pi = NULL;                     /* pi[i] = number of primes ≤ i */
static uint32_t *primes = NULL;              /* list of primes (1‑based order) */
static size_t primes_count = 0;              /* number of primes stored   */
static size_t primes_capacity = 0;           /* allocated capacity of primes[]   */
static idx_t sieve_limit = 0;                /* last index covered by sieve   */

/* Ensure that the sieve, pi array and primes list cover at least up to n. */
static void ensure_sieve(idx_t n)
{
    size_t required = (size_t)n + 1;  /* indices 0..n inclusive */
    if (required <= sieve_limit + 1 && is_composite) return;

    /* Reallocate is_composite array */
    unsigned char *new_is = (unsigned char *)realloc(is_composite, required);
    if (!new_is) abort();
    is_composite = new_is;

    /* Reallocate pi array */
    idx_t *new_pi = (idx_t *)realloc(pi, required * sizeof(idx_t));
    if (!new_pi) abort();
    pi = new_pi;

    /* Initialize new entries as prime candidates */
    size_t start_new = (size_t)sieve_limit + 1;
    if (start_new < 2) start_new = 2; /* start from 2 to avoid 0/1 */
    for (size_t i = start_new; i < required; ++i) is_composite[i] = 0;

    /* Mark composites in the new range using already known primes */
    for (size_t idx = 0; idx < primes_count; ++idx) {
        uint32_t p = primes[idx];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        size_t first;
        if (p2 > (uint64_t)sieve_limit) {
            first = (size_t)p2;
        } else {
            first = ((size_t)sieve_limit + 1 + p - 1) / p * p;
            if (first < p2) first = (size_t)p2;
        }
        for (size_t j = first; j < required; j += p)
            is_composite[j] = 1;
    }

    /* Find new primes and mark their multiples */
    for (size_t i = start_new; i < required; ++i) {
        if (!is_composite[i]) {
            /* Add new prime to list */
            if (primes_count == primes_capacity) {
                primes_capacity = primes_capacity ? primes_capacity * 2 : 256;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_capacity * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_count++] = (uint32_t)i;

            /* Mark multiples of this prime */
            uint64_t i2 = (uint64_t)i * (uint64_t)i;
            if (i2 < required) {
                for (size_t j = i2; j < required; j += i)
                    is_composite[j] = 1;
            }
        }
    }

    /* Compute pi array up to the new limit */
    if (required > 0) pi[0] = 0;
    if (required > 1) pi[1] = 0;
    for (size_t i = 2; i < required; ++i)
        pi[i] = pi[i - 1] + (!is_composite[i] ? 1 : 0);

    sieve_limit = (idx_t)(required - 1);
}

/* Ensure that at least 'need' primes are available in the primes array. */
static void ensure_primes_count(size_t need)
{
    if (need <= primes_count) return;

    /* Expand the sieve limit in powers of two until enough primes are found. */
    idx_t limit = sieve_limit + 1;
    if (limit < 2) limit = 2;
    while (primes_count < need) {
        limit *= 2;
        ensure_sieve(limit - 1);
    }
}

seq_t a(idx_t n)
{
    /* Build sieve up to index n (for primality test and pi(n)). */
    ensure_sieve(n);

    bool is_prime_idx = (n >= 2 && !is_composite[(size_t)n]);

    idx_t pi_n = pi[(size_t)n];            /* number of primes ≤ n           */
    idx_t nonprime_count = (n + 1) - pi_n; /* number of non‑prime indices ≤ n */

    size_t needed_primes = is_prime_idx ? (size_t)pi_n : (size_t)nonprime_count;

    /* Make sure we have that many primes in our list. */
    ensure_primes_count(needed_primes);

    /* Return the (needed_primes‑th) prime (1‑based index). */
    size_t idx = needed_primes - 1;      /* zero‑based index in primes array */
    return (seq_t)primes[idx];
}
