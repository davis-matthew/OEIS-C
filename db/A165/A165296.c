/*
 * A165296  The larger member of a prime pair (p,p+10000).
 * https://oeis.org/A165296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 948, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the larger member of a prime pair (p,p+10000). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Return the n‑th term of A165296: the larger prime of a prime pair
   with difference 10000 (offset 1).  The function is memoised so that
   repeated calls with arbitrary n are fast. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* stored terms */
    static size_t cache_size = 0;         /* number of terms stored */
    static size_t cache_cap  = 0;         /* allocated capacity of cache */
    static idx_t  next_p     = 2;         /* current candidate prime p */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        /* Find the next pair (p, p+10000) where both are prime. */
        while (!(is_prime(next_p) &&
                 is_prime(next_p + 10000))) {
            next_p++;
        }

        /* Ensure sufficient capacity in the cache. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {   /* allocation failed – return zero as a fallback */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the larger prime of the pair. */
        cache[cache_size] = (seq_t)(next_p + 10000);
        cache_size++;

        /* Advance to the next candidate prime. */
        next_p++;
    }

    return cache[n - 1];
}
