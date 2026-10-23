/*
 * A142738  Primes congruent to 11 mod 59.
 * https://oeis.org/A142738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 102, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 11 (mod 59).
   a(n) is the n-th prime p such that p ≡ 11 (mod 59). */
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"   /* prototype: bool is_prime_big(seq_t n); */

/* Return the n‑th prime congruent 11 (mod 59).  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined by offset – safety return */
        return 0;

    /* static cache of previously found primes */
    static seq_t *cache = NULL;   /* array of primes */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of primes stored */

    /* quick return if we already have the answer */
    if (n <= len)
        return cache[n - 1];

    /* start candidate after the last cached prime, or at 11 */
    seq_t cand = (len == 0) ? (seq_t)11 : cache[len - 1] + (seq_t)59;

    /* generate primes until we reach the requested index */
    while (len < n) {
        /* advance until the candidate is prime */
        while (!is_prime_big(cand))
            cand += (seq_t)59;

        /* grow cache if needed */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache && len > 0)
                memcpy(new_cache, cache, len * sizeof(seq_t));
            free(cache);
            cache = new_cache;
            cap   = new_cap;
        }

        /* store the found prime */
        cache[len++] = cand;
        cand += (seq_t)59;          /* next candidate in the AP */
    }

    return cache[n - 1];
}
