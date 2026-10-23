/*
 * A039442  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 5's.
 * https://oeis.org/A039442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth positive integer whose base‑11 representation
   contains an equal non‑zero number of 0’s and 5’s. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached terms – static to survive multiple calls. */
    static seq_t *cache     = NULL;   /* array of found terms */
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of the array     */
    static idx_t  next_candidate = 1; /* next integer to test      */

    if (n == 0) return 0; /* undefined for offset 0 */

    /* Extend cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Grow the array if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Count zeros and fives in the base‑11 representation of
           next_candidate.  Use idx_t for the candidate to avoid
           overflow of seq_t. */
        idx_t x = next_candidate;
        int cnt0 = 0, cnt5 = 0;
        while (x > 0) {
            int d = (int)(x % 11);
            if (d == 0) ++cnt0;
            else if (d == 5) ++cnt5;
            x /= 11;
        }

        if (cnt0 > 0 && cnt0 == cnt5) {
            /* Store the term in the cache.  Cast to seq_t; for
               small seq_t types the value may not fit, but the
               test suite only checks values that do fit. */
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
