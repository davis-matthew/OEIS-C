/*
 * A052424  Numbers k with no single-digit factors (apart from 1 and k).
 * https://oeis.org/A052424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k with no single‑digit proper factors (apart from 1 and k).
   Computed by a simple brute‑force search. */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* For indices below the offset, return 0 (behaviour unimportant). */
    if (n == 0) return (seq_t)0;

    /* Static variables to keep state between calls. */
    static seq_t  *cache      = NULL;   /* array of found terms      */
    static size_t cache_len  = 0;       /* number of terms stored    */
    static size_t cache_cap  = 0;       /* allocated capacity        */
    static idx_t  next_candidate = 1;   /* integer to test next      */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Test whether next_candidate satisfies the definition. */
        bool good = false;
        if (next_candidate <= 7) {
            /* 1,2,3,5,7 are explicitly allowed. */
            good = (next_candidate == 1) || (next_candidate == 2) ||
                   (next_candidate == 3) || (next_candidate == 5) ||
                   (next_candidate == 7);
        } else {
            /* For larger numbers, check divisibility by any integer
               2,…,9.  It is sufficient to test 2,3,5,7. */
            good = (next_candidate % 2 != 0) &&
                   (next_candidate % 3 != 0) &&
                   (next_candidate % 5 != 0) &&
                   (next_candidate % 7 != 0);
        }

        if (good) {
            /* Allocate or grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the accepted value. */
            cache[cache_len] = (seq_t)next_candidate;
            cache_len++;
        }
        next_candidate++;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
