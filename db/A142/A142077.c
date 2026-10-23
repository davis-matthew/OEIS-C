/*
 * A142077  Primes congruent to 2 mod 35.
 * https://oeis.org/A142077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds primes congruent to 2 mod 35 by incremental search and caches the
   results so that each element can be retrieved in O(1) time after it has
   been generated. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_generic.h"   /* provides bool is_prime_generic(idx_t) */

seq_t a(idx_t n)
{
    /* Cache of computed terms a(k), 1‑based index. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Next candidate of the form 2 + 35·m that has not yet been tested. */
    static idx_t next_candidate = 2;

    /* Grow the cache up to at least n elements. */
    while (len < (size_t)n)
    {
        idx_t cand = next_candidate;
        /* Skip until we hit the next prime ≡ 2 (mod 35). */
        while (!is_prime_generic(cand))
            cand += 35;                     /* same residue class */

        /* Expand the storage if necessary. */
        if (len == cap)
        {
            size_t new_cap = (cap == 0) ? 4 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)             /* allocation failed – give up */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the found prime. */
        cache[len++] = (seq_t)cand;

        /* Continue the search from the next candidate. */
        next_candidate = cand + 35;
    }

    /* Return the requested term (1‑based index). */
    return cache[(size_t)n - 1];
}
