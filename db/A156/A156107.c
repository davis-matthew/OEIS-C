/*
 * A156107  Primes p such that p + 144 is also prime.
 * https://oeis.org/A156107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p + 144 is also prime
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Prefer the helper routines supplied by the build system. */
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache for all prime pairs found so far. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The smallest candidate to examine next. */
    static idx_t next = 2;

    /* Fill the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t p = next;

        /* Find the next prime. */
        while (!is_prime(p))
            ++p;

        /* If both p and p + 144 are prime, store p. */
        if (is_prime(p + 144)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }

        /* Advance to the next candidate after p. */
        next = p + 1;
    }

    return cache[n - 1];
}
