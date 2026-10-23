/*
 * A100636  4^(p^2) where p is prime.
 * https://oeis.org/A100636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(p_n^2) (p_n = n‑th prime, n≥1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc, abort */
#include <stdbool.h>   /* bool */
#include "../utils/pow_seq.h"

/* Dynamic cache of primes for fast nth_prime queries. */
static uint64_t *prime_list   = NULL; /* array of known primes */
static size_t     prime_count = 0;    /* number of primes stored   */
static size_t     prime_cap   = 0;    /* capacity of the array    */
static uint64_t   next_candidate = 2; /* next integer to test     */

/* Return the n-th prime (1‑based). */
static uint64_t nth_prime(idx_t n)
{
    /* Expand the cache until it contains at least n primes. */
    while (prime_count < (size_t)n)
    {
        uint64_t cand   = next_candidate;
        bool     is_prime = true;

        /* Trial division by previously found primes. */
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= cand; ++i)
        {
            if (cand % prime_list[i] == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* Ensure sufficient capacity. */
            if (prime_count == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 1024;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp)
                {
                    abort(); /* out of memory */
                }
                prime_list = tmp;
            }
            prime_list[prime_count++] = cand;
        }
        ++next_candidate;
    }
    return prime_list[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
    {
        /* Undefined for n = 0; return 0 as a neutral placeholder. */
        return (seq_t)0;
    }

    /* Obtain the n‑th prime. */
    uint64_t p = nth_prime(n);

    /* Compute exponent: 2 × p² (since 4^(p²) = 2^(2p²)). */
    idx_t exponent = (idx_t)((uint64_t)2 * p * p);

    /* Return 2^exponent.  pow_seq handles big integers if seq_t is big. */
    return pow_seq((seq_t)2, exponent);
}
