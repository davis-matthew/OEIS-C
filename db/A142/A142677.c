/*
 * A142677  Primes congruent to 20 mod 57.
 * https://oeis.org/A142677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 20 (mod 57)
 * Range:     u8 n <= 1, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Primality test (provided in the utils library) */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* --- Memoization cache --------------------------------------------------- */
    static seq_t  *cache      = NULL;   /* stored primes                */
    static size_t cache_len   = 0;       /* number of primes stored      */
    static size_t cache_cap   = 0;       /* current allocation (bytes/elem) */
    static idx_t  next_candidate = 0;    /* next number to test (20,77,…) */

    /* ---- Initialisation ----------------------------------------------------- */
    if (cache == NULL) {
        cache_cap = n > 128 ? n : 128;   /* start with a modest capacity   */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 20;            /* first number of the residue class */
    }

    /* ---- Grow the cache if we need more space ------------------------------- */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        while (new_cap < (size_t)n) new_cap <<= 1; /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – unlikely in tests */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* ---- Generate primes until we have the requested index ----------------- */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate))          /* test primality */
            cache[cache_len++] = (seq_t)next_candidate;
        next_candidate += 57;                  /* next candidate in residue class 20 mod 57 */
    }

    /* ---- Return the answer --------------------------------------------------- */
    return cache[n - 1];
}
