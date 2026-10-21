/*
 * A226369  Number of tilings of a 5 X n rectangle using integer-sided square
 *          tiles of area > 1.
 * https://oeis.org/A226369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + 2*a(n-6) for n > 5 a(0) = 1 a(1) = a(2) = a(3) =
 *            a(4) = 0 a(5) = 1 (This recurrence follows from the generating
 *            function 1/(1‑x⁵-2x⁶).)
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 122, u64 n <= 236,
 *            u128 n <= 464, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Resize the cache if the requested index is beyond the current size */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort();  /* failure to allocate */

        /* Initialise the newly added entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                new_cache[i] = 1;
            } else if (i <= 4) {
                new_cache[i] = 0;
            } else if (i == 5) {
                new_cache[i] = 1;
            } else {
                /* Recurrence: a(n) = a(n-5) + 2*a(n-6) */
                new_cache[i] = new_cache[i - 5] + (new_cache[i - 6] * 2);
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
