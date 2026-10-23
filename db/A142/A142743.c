/*
 * A142743  Primes congruent to 16 mod 59.
 * https://oeis.org/A142743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p mod 59 == 16
 * Range:     u8 n <= 1, u16 n <= 96, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
    static seq_t last_prime = 0;

    if (n == 0) /* sequence is 1‑based; undefined for 0, return 0 */
        return 0;

    /* Return cached value if already known */
    if (cache_count >= (size_t)n)
        return cache[n - 1];

    /* Starting candidate for the next prime of the required form */
    seq_t cand = (cache_count == 0) ? 16 : last_prime + 59;

    /* Generate primes until we have at least n of them */
    while (cache_count < (size_t)n)
    {
        /* Find next prime that is 16 mod 59 */
        while (!is_prime_big(cand))
            cand += 59;

        /* Ensure enough space in the cache */
        if (cache_count >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found prime */
        cache[cache_count++] = cand;
        last_prime = cand;
        cand += 59; /* next candidate to test */
    }

    return cache[n - 1];
}
