/*
 * A142117  Primes congruent to 8 mod 37.
 * https://oeis.org/A142117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime congruence: a(n) are the primes ≡ 8 mod 37 (offset 1) */
/* The function computes successive primes in this residue class by incremental
   trial‑division and memoises the results so that later calls do not
   recompute previously found terms.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* reject non‑positive indices */
    if (n == 0) return 0;

    static seq_t *cache = NULL;    /* memoised results */
    static size_t cache_cap = 0;   /* current capacity of cache */
    static idx_t cache_len = 0;    /* number of elements stored */

    /* grow cache until it holds at least n entries */
    while (cache_len < n) {
        uint64_t candidate;

        /* first element: the smallest prime ≡ 8 (mod 37) */
        if (cache_len == 0) {
            candidate = 193;
        } else {
            /* next candidate in the residue class */
            candidate = (uint64_t)cache[cache_len - 1] + 37;
        }

        /* find the next prime in this residue class */
        while (!is_prime((idx_t)candidate)) {
            candidate += 37;
        }

        /* extend the cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed: return 0 as error */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)candidate;
    }

    return cache[n - 1];
}
