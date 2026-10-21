/*
 * A051647  Primes p such that 210*p + 1 is also prime.
 * https://oeis.org/A051647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 31, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that 210*p + 1 is prime (OEIS A051647). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"
#include "../utils/grow_prime.h"

/* Cache for the sequence values ------------------------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* Cache for primes used in trial division --------------------------- */
static uint64_t *prime_cache = NULL;
static size_t prime_len      = 0;
static size_t prime_cap      = 0;

/* Next integer to test for primality -------------------------------- */
static uint64_t next_candidate = 2;

/* Ensure the sequence cache is large enough -------------------------- */
static void grow_seq(size_t *cap, seq_t **array, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
    if (newcap < needed) newcap = needed;
    *array = seq_realloc(*array, *cap, newcap);
    *cap   = newcap;
}

/* Test whether n is prime using the primes already in prime_cache ---- */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_cache[i];
        if (p > n / p) break;          /* p*p > n   */
        if (n % p == 0) return false;
    }
    return true;
}

/* Sequence entry ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */

    while (seq_len < (size_t)n) {
        /* Find the next prime number. */
        uint64_t cand = next_candidate;
        while (true) {
            if (is_prime_candidate(cand)) {
                /* Store the new prime in prime_cache. */
                grow_prime(&prime_cap, &prime_cache, prime_len + 1);
                prime_cache[prime_len++] = cand;

                /* Test 210*cand + 1 for primality. */
                seq_t test = (seq_t)cand * (seq_t)210 + (seq_t)1;
                if (is_prime_big(test)) {
                    grow_seq(&seq_cap, &seq_cache, seq_len + 1);
                    seq_cache[seq_len++] = (seq_t)cand;
                    if (seq_len >= (size_t)n) break;
                }
            }
            ++cand;
        }
        next_candidate = cand + 1;
    }
    return seq_cache[n - 1];
}
