/*
 * A067832  Primes p such that sigma(p-6) > p.
 * https://oeis.org/A067832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4792, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes OEIS sequence A067832:
   the n‑th prime p such that sigma(p‑6) > p,
   by brute‑force prime search and divisor‑sum testing. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num); */
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Local cache of primes satisfying the condition. */
static uint64_t *found_primes = NULL;      /* prime values (uint64_t) */
static size_t   found_count = 0;           /* how many have been found */
static size_t   found_cap   = 0;           /* current capacity of the array */

/* The next odd number to test; we start just above 6 to avoid trivial cases. */
static uint64_t current_candidate = 7;

/* Compute the sum-of-divisors function sigma for a 64‑bit integer. */
static uint64_t sigma_u64(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64(n, primes, exps, &cnt);   /* factorisation */

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute (p^(e+1)-1)/(p-1) by iterative multiplication to avoid overflow. */
        uint64_t term = 1;       /* accounts for p^0 */
        uint64_t power = 1;      /* p^0 */

        for (uint64_t k = 0; k < e; ++k) {  /* add p^1 ... p^e */
            power *= p;
            term += power;
        }
        result *= term;
    }
    return result;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Generate primes until we have at least n accepted ones. */
    while (found_count < (size_t)n) {
        /* Test current_candidate for primality. */
        if (is_prime_small(current_candidate)) {
            uint64_t p = current_candidate;
            uint64_t m = p - 6;
            uint64_t sigma = sigma_u64(m);

            if (sigma > p) {
                /* Store this prime in the cache, growing the array if needed. */
                if (found_count == found_cap) {
                    size_t new_cap = found_cap ? found_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(found_primes, new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);   /* unrecoverable memory exhaustion */
                    found_primes = tmp;
                    found_cap = new_cap;
                }
                found_primes[found_count++] = p;
            }
        }

        /* Advance to the next odd number. */
        current_candidate += 2;
    }

    /* Return the requested term (1‑based index). */
    return (seq_t)found_primes[(size_t)n - 1];
}
