/*
 * A142016  Primes congruent to 12 mod 31.
 * https://oeis.org/A142016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 223, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142016: primes congruent to 12 (mod 31) – found by a search
   of candidates 31·k+12 with primality tested by is_prime(). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p such that p ≡ 12 (mod 31).
   Indices start at 1; a(0) is undefined and returns 0. */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of stored terms */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t next_k      = 1;   /* next k in 31·k+12 */

    if (n == 0)
        return (seq_t)0;            /* a(0) is undefined */

    /* Return cached value if available. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Allocate more space if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
            if (!tmp)                /* allocation failure */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Candidate of the form 31·k+12. */
        idx_t candidate = (idx_t)31 * next_k + (idx_t)12;
        ++next_k;

        /* Store if prime. */
        if (is_prime(candidate))
            cache[cache_len++] = (seq_t)candidate;
    }

    return cache[n - 1];
}
