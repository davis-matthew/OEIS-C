/*
 * A142314  Primes congruent to 4 mod 45.
 * https://oeis.org/A142314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is congruent to 4 (mod 45)
 * Range:     u8 n <= 2, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 969 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental search, caching the    */
/* results so that any order of calls is handled efficiently.          */
/* It relies on the provided is_prime(idx_t) and grow_seq() utilities. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;          /* array of primes found  */
    static size_t cache_cap = 0;         /* allocated capacity     */
    static size_t cache_len = 0;         /* number of elements filled */
    static idx_t next_candidate = 139;   /* first prime ≡ 4 (mod 45) */

    /* Ensure we have at least 'n' terms in the cache */
    while (cache_len < n) {
        /* Test candidate for primality */
        if (is_prime(next_candidate)) {
            /* Grow the cache if necessary */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            /* Store the prime as seq_t (conversion is safe for n ≤ 10000) */
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* Move to the next number of the form 4 (mod 45) */
        next_candidate += 45;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
