/*
 * A215353  Primes congruent to {2, 3, 4, 6} mod 19.
 * https://oeis.org/A215353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, matches_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p ≡ 2, 3, 4, or 6 (mod 19) */

/*  The function maintains a static cache of all qualifying primes it
    has found so far.  It grows this cache with standard malloc /
    realloc calls, which are valid for integral seq_t types and
    arbitrary‑precision classes that have a converting constructor from
    uint64_t.  Each call finds primes as needed and returns the
    n‑th prime in the sequence.  The algorithm runs in O(n log n) time
    for the n-th term, well below the required limits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */
#include "../utils/matches_residue.h"

static uint64_t *prime_cache = NULL;   /* cached primes as uint64_t     */
static idx_t    cache_len   = 0;       /* number of primes stored       */
static size_t   cache_cap   = 0;       /* allocated capacity in Uint64  */
static uint64_t next_candidate = 2;    /* next integer to examine      */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap <<= 1;          /* double until large enough    */
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp && new_cap != 0)
            abort();                /* allocation failure            */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Undefined for n = 0; return 0 as a sentinel. */
    if (n == 0)
        return 0;

    /* Grow cache until it contains the n-th qualifying prime. */
    while (cache_len < n) {
        uint64_t cand = next_candidate;
        ++next_candidate;          /* next candidate for future calls */

        if (is_prime(cand) && matches_residue(cand)) {
            ensure_capacity(cache_len + 1);
            prime_cache[cache_len++] = cand;
        }
    }
    /* Convert stored uint64_t to seq_t before returning. */
    return (seq_t)prime_cache[n - 1];
}
