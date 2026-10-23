/*
 * A037437  Positive numbers having the same set of digits in base 6 and base
 *          10.
 * https://oeis.org/A037437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 211, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   same_digits_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive numbers having the same set of digits in base 10 and base 6.
   a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5, ...        */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/same_digits_set.h"

/* Returns the n‑th element of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms */
    static idx_t cache_len = 0;      /* number of stored terms */
    static idx_t cache_cap = 0;      /* allocated capacity of cache */
    static idx_t next_search = 1;    /* next candidate integer to test */

    if (n == 0)
        return 0;                    /* off‑by‑offset behaviour is irrelevant */

    /* ensure we have computed at least the first n terms */
    while (cache_len < n) {
        /* extend cache if needed */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* test candidates until we find the next term */
        while (!same_digits_set(next_search))
            ++next_search;

        cache[cache_len++] = (seq_t)next_search;
        ++next_search;                /* advance for next search */
    }

    return cache[n - 1];
}
