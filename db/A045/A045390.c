/*
 * A045390  Primes congruent to {1, 2} mod 8.
 * https://oeis.org/A045390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is 2 or ≡ 1 (mod 8)
 * Range:     u8 n <= 11, u16 n <= 1615, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A045390: primes congruent to 1 or 2 mod 8) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for trial‑division primes and the sequence values */
static uint64_t *primes      = NULL;   /* all primes found so far   */
static size_t   primes_len  = 0;
static size_t   primes_cap  = 0;

static uint64_t *cache_uint  = NULL;   /* terms of the sequence, stored as uint64_t   */
static size_t   cache_len   = 0;
static size_t   cache_cap   = 0;

static uint64_t next_candidate = 3;   /* start scanning at 3 (odd only) */
static bool initialized = false;

/* Ensure that the sequence has at least n terms stored in cache_uint. */
static void ensure_n(idx_t n)
{
    if (n == 0) return;          /* offset starts at 1 */

    if (!initialized) {          /* one‑time setup */
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_len = 1;

        cache_cap = 16;
        cache_uint = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
        cache_uint[0] = 2;      /* prime 2 is a(1) */
        cache_len = 1;

        initialized = true;
    }

    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* trial division using known primes up to sqrt(cand) */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store the new prime in the exhaustive prime list */
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;

            /* if it matches the sequence criterion, cache it */
            if ((cand & 7ULL) == 1ULL) {   /* cand % 8 == 1 */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache_uint = (uint64_t *)realloc(cache_uint, cache_cap * sizeof(uint64_t));
                }
                cache_uint[cache_len++] = cand;
            }
        }

        next_candidate += 2;  /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    ensure_n(n);
    return (seq_t)cache_uint[(size_t)n - 1];
}
