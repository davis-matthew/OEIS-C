/*
 * A105443  a(n) = the smallest prime p such that p+4 and q+4 are prime; q is
 *          the n-th prime after p.
 * https://oeis.org/A105443
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that p+4 and the n‑th prime
 *            after p +4 are prime. The sequence is defined for n ≥ 2; indices
 *            < 2 return 0
 * Range:     u8 n <= 32, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

static uint64_t *prime_list   = NULL;   /* array of primes */
static idx_t      prime_count = 0;      /* number of primes stored */
static idx_t      prime_cap   = 0;      /* allocated capacity of prime_list */
static uint64_t   next_candidate = 2;   /* next number to test for primality */

static void ensure_prime_cap(idx_t needed)
{
    if (prime_cap >= needed)
        return;
    idx_t new_cap = prime_cap ? prime_cap*2 : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void ensure_prime_count(idx_t idx)
{
    if (prime_count > idx)
        return;
    ensure_prime_cap(prime_count + 1);   /* at least one more space needed */
    while (prime_count <= idx)
    {
        if (is_prime(next_candidate))
        {
            prime_list[prime_count++] = next_candidate;
        }
        /* advance to the next odd number beyond 2, after 2 we skip evens */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/** Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* behaviour for indices before offset */

    /* Initialise prime list on first call */
    if (!prime_list)
    {
        prime_list = (uint64_t *)malloc(128 * sizeof(uint64_t));
        if (!prime_list) abort();
        prime_cap  = 128;
        prime_count = 0;
        next_candidate = 2;
        /* generate first prime */
        ensure_prime_count(0);
    }

    /* Scan primes sequentially */
    for (idx_t i = 0;; ++i)
    {
        /* Make sure we have enough primes for this candidate */
        ensure_prime_count(i);          /* ensures p = prime_list[i] exists */

        /* Ensure we have the nth prime after p */
        ensure_prime_count(i + n);

        uint64_t p = prime_list[i];
        uint64_t q = prime_list[i + n];

        if (!is_prime(p + 4))
            continue;
        if (!is_prime(q + 4))
            continue;

        return (seq_t)p;
    }
}
