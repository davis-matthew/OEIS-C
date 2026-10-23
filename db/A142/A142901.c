/*
 * A142901  Primes congruent to 22 mod 63.
 * https://oeis.org/A142901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime ≡ 22 (mod 63)
 * Range:     u8 n <= 1, u16 n <= 181, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142901 – primes congruent to 22 modulo 63 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of elements stored */

    /* Candidate index k for numbers of the form 63*k+22; start at k=3 because
       3*63+22 = 211 is the first required prime. */
    static idx_t next_k = 3;

    if (n == 0)               /* sequence is 1‑based; undefined for n=0 */
        return 0;

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < n) {

        /* Compute the next candidate number. */
        seq_t candidate = (seq_t)63 * (seq_t)next_k + (seq_t)22;

        /* Test primality. is_prime_big works for any seq_t type. */
        if (is_prime_big(candidate)) {
            /* Ensure there is room for the new element. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* Explicit cast required for C++ compatibility. */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;  /* store the prime */
        }

        ++next_k;  /* Move to the next number ≡ 22 mod 63. */
    }

    return cache[n - 1];
}
