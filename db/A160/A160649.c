/*
 * A160649  a(1)=2. a(n) = a(n-1) + A001222(a(n-1)); where A001222(m) is the
 *          sum of prime-factorization exponents of m (or, A001222(m) is the
 *          number of primes dividing m, with multiplicity).
 * https://oeis.org/A160649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; a(n) = a(n-1) + A001222(a(n-1)) (A001222 = sum of
 *            prime‑factorization exponents)
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/count_prime_factors.h"

/* Cached values of the sequence, zero‑based indexed: a(1) is cache[0] */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of entries currently stored */

/* Ensure that the cache contains values up to index n (1‑based). */
static void ensure_cache_upto(idx_t n)
{
    if ((size_t)n <= cache_len)        /* already computed */
        return;

    /* Expand the cache to hold n elements. */
    grow_seq(&cache, &cache_cap, (size_t)n);

    /* Initialise a(1) if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;          /* a(1) = 2 */
        cache_len = 1;
    }

    /* Compute all new terms incrementally. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i-1];
        seq_t inc  = count_prime_factors(prev);
        cache[i] = prev + inc;
    }
    cache_len = (size_t)n;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Assume n >= 1, as required by the sequence definition. */
    ensure_cache_upto(n);
    return cache[(size_t)n - 1];
}
