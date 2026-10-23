/*
 * A039544  Numbers whose base-12 representation has the same nonzero number
 *          of 6's and 7's.
 * https://oeis.org/A039544
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

/* Sequence A039544: Numbers whose base‑12 representation has the same
   non‑zero number of 6's and 7's.  The function returns the n‑th such
   number (a(1) = 79, a(2) = 90, ...).  It looks for numbers by
   sequentially scanning integers and storing found terms in a static
   cache so that repeated calls are efficient. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;

    /* the next integer to be examined */
    static seq_t next = 1;

    /* Helper to grow the cache */
    if (cache == NULL) {
        cache_capacity = 128;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    }
    while (cache_size < n) {
        /* Find the next number satisfying the condition */
        seq_t x = next;
        int c6 = 0, c7 = 0;
        while (x) {
            seq_t d = x % 12;
            if (d == 6) {
                ++c6;
            } else if (d == 7) {
                ++c7;
            }
            x /= 12;
        }
        if (c6 == c7 && c6 > 0) {
            /* store the term in the cache */
            if (cache_size == cache_capacity) {
                cache_capacity *= 2;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = next;
        }
        ++next;
    }
    return cache[n - 1];
}
