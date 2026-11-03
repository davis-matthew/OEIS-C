/*
 * A142413  Primes congruent to 47 mod 48.
 * https://oeis.org/A142413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 411, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 929 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p with p mod 48 == 47.
   Uses an incremental search with memoisation.
   The function works for any seq_t and idx_t defined by the build. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    /* static cache of found primes */
    static seq_t *cache      = NULL;   /* array holding the primes */
    static size_t cache_cap = 0;       /* allocated capacity of cache   */
    static size_t cache_len = 0;       /* number of primes stored       */
    static idx_t  next_candidate = 47; /* first candidate with residue 47 */

    /* extend the cache until it contains at least n elements */
    while (cache_len < n)
    {
        /* test current candidate for primality */
        if (is_prime(next_candidate))
        {
            /* ensure space for the new element */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* advance to the next number with the same residue */
        next_candidate += 48;
    }

    /* return the requested term (1‑based indexing) */
    return cache[n - 1];
}
