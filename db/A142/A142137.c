/*
 * A142137  Primes congruent to 28 mod 37.
 * https://oeis.org/A142137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 180, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = the n-th prime p such that p ≡ 28 (mod 37).
   We generate primes by testing successive integers with is_prime_big,
   storing those that satisfy the congruence.  Results are cached
   so that arbitrary order of calls is handled efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cached     = NULL;
    static size_t cached_cap = 0;
    static size_t cached_sz  = 0;

    /* smallest integer to start checking from */
    static seq_t candidate = 2;

    /* offset is 1, values below are irrelevant */
    if (n == 0) return 0;

    /* return cached value if already computed */
    if ((size_t)n <= cached_sz)
        return cached[n - 1];

    /* initialise cache on first use */
    if (cached_cap == 0) {
        cached_cap = 8;
        cached = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cached) abort();          /* allocation failure */
    }

    /* generate primes until we have the n-th one */
    while (cached_sz < (size_t)n) {
        /* find the next prime */
        while (!is_prime_big(candidate)) {
            ++candidate;
        }

        /* if it satisfies the congruence, store it */
        if (candidate % (seq_t)37 == (seq_t)28) {
            if (cached_sz >= cached_cap) {       /* grow array */
                cached_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cached,
                                              cached_cap * sizeof(seq_t));
                if (!tmp) abort();
                cached = tmp;
            }
            cached[cached_sz++] = candidate;
        }

        /* move on to the next integer after the current prime */
        ++candidate;
    }

    return cached[n - 1];
}
