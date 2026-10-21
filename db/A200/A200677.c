/*
 * A200677  Smallest semiprime such that the sum of the two prime factors
 *          equals n, or zero if impossible.
 * https://oeis.org/A200677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest semiprime whose two prime factors sum to n.
   For a given n≥1, the function returns the minimal product p*q
   where p and q are primes (possibly equal) and p+q=n.
   If no such pair exists, it returns 0. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

static uint64_t *prime_list = NULL;   /* primes up to current limit */
static size_t prime_count = 0;        /* number of primes in prime_list */
static idx_t  prime_limit = 0;       /* current prime sieve limit */

/* Helper to ensure we have primes up to 'limit' (inclusive). */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                /* already have enough primes */

    /* Build or extend the prime list up to the new limit. */
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;            /* no pair of primes sums to ≤3 */

    idx_t sieve_limit = n - 2;    /* largest possible q = n - p ≤ n-2 */
    ensure_primes(sieve_limit);

    uint64_t best = 0;            /* 0 means "not found yet" */
    uint64_t p, q;

    /* Iterate over primes p ≤ n/2 */
    for (size_t i = 0; i < prime_count; ++i) {
        p = prime_list[i];
        if (p > n / 2)
            break;

        q = n - p;
        if (q < 2)
            continue;

        /* Check if q is prime */
        if (!is_prime_small((seq_t)q))
            continue;

        uint64_t prod = p * q;
        if (best == 0 || prod < best)
            best = prod;
    }

    return (seq_t)best;   /* best is 0 if none found */
}
