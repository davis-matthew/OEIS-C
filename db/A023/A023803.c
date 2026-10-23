/*
 * A023803  Xenodromes: all digits in base 8 are different.
 * https://oeis.org/A023803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   octal_distinct_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023803 – Xenodromes: all digits in base 8 are different.
 *
 * The value a(n) is the nth non‑negative integer (counting from 1) whose
 * representation in octal uses no repeated digit.  The sequence starts:
 * 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, …
 *
 * The implementation keeps a static cache of the values that have
 * already been generated.  On each call the cache is extended until it
 * contains at least n elements.  The next candidate integer is tested
 * for distinct octal digits by iterating over its octal digits and
 * using a simple 8‑bit mask.  Capacity is doubled as a power of two
 * to keep the amortised insertion time O(1).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/octal_distinct_idx.h"

/* The sequence function. */
seq_t a(idx_t n)
{
    /* static state: cache of computed terms */
    static seq_t *cache      = NULL;   /* array of terms          */
    static idx_t cache_len   = 0;      /* number of terms stored  */
    static idx_t cache_cap   = 0;      /* allocated capacity      */
    static idx_t next_candidate = 0;   /* next integer to test    */

    if (n < 1)                          /* undefined for n < 1      */
        return 0;

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* find next integer with distinct octal digits */
        while (!octal_distinct_idx(next_candidate))
            next_candidate++;

        /* grow the array if necessary */
        if (cache_len == cache_cap) {
            cache_cap = (cache_cap == 0) ? 4 : (cache_cap << 1);
            cache = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        }

        /* store the term */
        cache[cache_len] = (seq_t)next_candidate;
        cache_len++;
        next_candidate++;
    }

    return cache[n - 1];
}
