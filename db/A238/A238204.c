/*
 * A238204  Even numbers n such that the difference with the preceding prime
 *          is prime.
 * https://oeis.org/A238204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth even number such that the difference * with the
 *            greatest prime below it is also prime
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"
#include "../utils/build_primes_upto.h"

static seq_t  *cache      = NULL;     /* storage for sequence values   */
static size_t cache_len  = 0;          /* number of stored values       */
static size_t cache_cap  = 0;          /* allocated capacity            */

static uint64_t *primes = NULL;        /* primes up to a fixed limit    */
static size_t   prime_cnt = 0;          /* number of primes              */
static bool     primes_built = false;  /* primes array ready?           */

static seq_t  next_candidate = 10;     /* next even number to test      */
static size_t prime_idx = 0;           /* index of largest prime < candidate */

/* Return the nth element of A238204 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against 0 */

    /* Build primes once, up to a limit that safely covers n≈10000. */
    if (!primes_built) {
        const idx_t PRIME_LIMIT = 200000; /* enough for n up to ~10000 */
        build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);
        primes_built = true;

        /* initialise prime_idx for the first candidate (10) */
        prime_idx = 0;
        while (prime_idx + 1 < prime_cnt && primes[prime_idx + 1] < next_candidate)
            ++prime_idx;
    }

    /* Extend the sequence until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        seq_t diff = cand - (seq_t)primes[prime_idx];

        if (is_prime_small(diff)) {
            /* grow cache if necessary */
            if (cache_len >= cache_cap) {
                cache_cap = (cache_cap ? cache_cap * 2 : 8);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }

        /* move to the next even number */
        next_candidate += 2;

        /* advance prime_idx if the next even number passes a prime */
        while (prime_idx + 1 < prime_cnt && primes[prime_idx + 1] < next_candidate)
            ++prime_idx;
    }

    return cache[(size_t)n - 1];
}
