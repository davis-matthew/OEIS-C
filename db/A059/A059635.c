/*
 * A059635  Primes p such that x^56 = 2 has no solution mod p, but x^28 = 2
 *          has a solution mod p.
 * https://oeis.org/A059635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A059635 – primes p such that 2 is a 28‑th power residue mod p
 *            but not a 56‑th power residue. The function searches primes in
 *            ascending order, stores those satisfying the condition in a
 *            static cache, and returns the *n*‑th stored prime
 * Range:     u8 none, u16 n <= 346, u32 n <= 950, u64 n <= 950, u128 n <= 950,
 *            big any n
 * Helpers:   gcd, is_prime, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cache of primes found so far */
static size_t cache_len = 0;     /* number of elements in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache     */
static uint64_t next_candidate = 3;  /* next number to test for primeness */

/* Return the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n primes. */
    while (cache_len < n) {
        /* Find the next prime ≥ next_candidate. */
        uint64_t cand = next_candidate;
        while (!is_prime(cand)) {
            cand += 2;              /* skip even numbers */
        }

        /* Prime found.  Update next_candidate to the following odd number. */
        next_candidate = cand + 2;

        /* Work with seq_t representations to handle arbitrary precision. */
        seq_t p       = (seq_t)cand;
        seq_t pminus1 = p - (seq_t)1;

        /* Compute gcd with 28 and 56. */
        seq_t d28 = gcd(pminus1, (seq_t)28);
        seq_t d56 = gcd(pminus1, (seq_t)56);

        /* Exponents for the power residue tests. */
        uint64_t exp28 = (uint64_t)(pminus1 / d28);
        uint64_t exp56 = (uint64_t)(pminus1 / d56);

        /* Check the residue conditions. */
        seq_t pow28 = mod_pow((seq_t)2, exp28, p);
        seq_t pow56 = mod_pow((seq_t)2, exp56, p);

        if (pow28 == (seq_t)1 && pow56 != (seq_t)1) {
            /* Ensure the cache has room. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = seq_realloc(cache, cache_cap, new_cap);
                if (!cache) {
                    /* Allocation failed – abort. */
                    exit(EXIT_FAILURE);
                }
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
