/*
 * A142908  Primes congruent to 34 mod 63.
 * https://oeis.org/A142908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 61, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 34 mod 63 (i.e. primes p with p ≡ 34 (mod 63)).
   The sequence is defined as a(n) = the n-th prime of the form 63k + 34,
   for k ≥ 0. The OEIS offset is 1. */

/* Include helper for primality test on generic seq_t values. */
#include "../utils/is_prime_big.h"
#include <stdlib.h>
#include <stdint.h>

/* a(n) returns the n-th such prime, where n ≥ 1. */
seq_t a(idx_t n)
{
    /* Static cache of already found primes. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t sz      = 0;    /* number of primes stored */
    static idx_t  next_m  = 0;    /* next k value to test */

    /* Max value that can be represented in seq_t. */
    const seq_t max_seq = (seq_t)(-1);               /* unsigned max */
    const unsigned long long max_seq_ul = (unsigned long long)max_seq;

    /* Ensure the cache has initial capacity. */
    if (cap == 0) {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate primes until the cache contains at least n entries. */
    while (sz < n) {
        /* Compute candidate 63*next_m + 34 using a wider type to avoid overflow. */
        const unsigned long long cand_ul = 63ULL * (unsigned long long)next_m + 34ULL;

        /* Only consider the candidate if it fits into seq_t. */
        if (cand_ul <= max_seq_ul) {
            const seq_t cand = (seq_t)cand_ul;
            if (is_prime_big(cand)) {
                /* Grow cache if necessary. */
                if (sz == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[sz++] = cand;
            }
        }
        ++next_m;
    }

    /* Return the (n-1)-th element (since n is 1‑based). */
    return cache[n - 1];
}
