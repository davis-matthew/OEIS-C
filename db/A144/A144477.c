/*
 * A144477  a(n) = minimal number of 0's that must be changed to 1's in the
 *          binary expansion of the n-th prime in order to make it into a
 *          palindrome.
 * https://oeis.org/A144477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, floor_log2_uint64,
 *            pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A144477
 *
 *  Formula: a(n) = number of mismatched bit pairs between
 *  the binary expansion of the n-th prime and its reversal.
 *
 *  a(n) is half the Hamming distance between prime(n) and the
 *  reversal of its binary representation.
 *
 *  Sequence is computed by:
 *    1. Determining the n-th prime.
 *    2. Counting differing pairs of bits in its binary form.
 *    3. Returning that count.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_uint.h"   /* for pow_uint, not strictly needed but harmless */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_log2_uint64.h"

static uint64_t *primes_cache = NULL;  /* cached list of primes */
static size_t primes_cnt   = 0;        /* number of cached primes */

/* Initialises the prime cache up to at least the 10000-th prime (≈104729).
   The chosen limit guarantees at least 17984 primes (π(200000)).
   This covers all n required by the tests. */
static void init_primes_cache(void)
{
    static const uint64_t PRIME_LIMIT = 200000;   /* > 17984 primes */

    if (primes_cache != NULL)
        return; /* already initialised */

    build_primes_upto(PRIME_LIMIT, &primes_cache, &primes_cnt);
}

/* Return the n-th prime (1‑based).  n must be >= 1. */
static inline uint64_t nth_prime(idx_t n)
{
    init_primes_cache();
    /* If the requested index exceeds the cached primes (unlikely), extend the cache */
    if ((size_t)n > primes_cnt) {
        /* Re‑compute with a larger limit */
        uint64_t new_limit = (uint64_t)n * ((uint64_t)n + 1); /* generous over‑approximation */
        free(primes_cache);
        primes_cache = NULL;
        primes_cnt = 0;
        build_primes_upto(new_limit, &primes_cache, &primes_cnt);
    }
    return primes_cache[n - 1];
}

/* Computes the number of pairs of bits that differ between the binary
 * representation of p and its reversal.  This is the Hamming distance
 * divided by two, i.e. the minimal number of 0→1 flips required. */
static inline seq_t palin_change_count(uint64_t p)
{
    /* Bit length of the number (p > 0). */
    idx_t bl = floor_log2_uint64(p) + 1;   /* type idx_t is uint64_t */

    seq_t mismatches = 0;
    idx_t i, j;
    for (i = 0, j = bl - 1; i < j; ++i, --j) {
        uint64_t bi = (p >> i) & 1ULL;
        uint64_t bj = (p >> j) & 1ULL;
        if (bi != bj)
            ++mismatches;
    }

    return mismatches;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for 0, return 0 just in case */

    uint64_t p = nth_prime(n);
    return palin_change_count(p);
}
