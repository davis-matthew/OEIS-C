/*
 * A039200  Numbers whose base-11 representation has the same number of 6's
 *          and 7's.
 * https://oeis.org/A039200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers whose base‑11 representation has the same number of 6's and 7's.
   The function searches integers sequentially and caches results; no closed formula is known. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff v has equal numbers of digits 6 and 7 in its base‑11 representation. */
static bool equal_6_7(seq_t v) {
    unsigned int c6 = 0, c7 = 0;
    for (seq_t x = v; x > 0; x /= 11) {
        seq_t r = x % 11;
        if (r == 6) ++c6;
        else if (r == 7) ++c7;
    }
    return c6 == c7;
}

seq_t a(idx_t n) {
    /* Cache of previously found terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0; /* number of cached terms */
    static size_t cache_cap  = 0; /* allocated capacity */

    /* Grow the cache until we have at least n terms */
    while (cache_size < n) {
        seq_t candidate = (cache_size == 0 ? 0 : cache[cache_size - 1] + 1);

        /* Find the next qualifying number */
        while (!equal_6_7(candidate))
            ++candidate;

        /* Resize the cache array if needed */
        if (cache_size >= cache_cap) {
            size_t new_cap = (cache_cap == 0 ? 1 : cache_cap * 2);
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) abort(); /* allocation failure */
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* Store the found term */
        cache[cache_size++] = candidate;
    }

    /* Return the n-th term (1‑based indexing) */
    return cache[n - 1];
}
