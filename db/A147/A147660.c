/*
 * A147660  Expansion of 1/(1-x-x^2+x^9-x^11).
 * https://oeis.org/A147660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 47, u64 n <= 93, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) - a(n-9) + a(n-11),
   with a(0) = 1, a(1) = 1. This implements the
   expansion of 1 / (1 - x - x^2 + x^9 - x^11). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* allocated capacity */
    static size_t end = 0;     /* last computed index (inclusive) */

    /* initial allocation and base values */
    if (cache == NULL) {
        cap = 16;                       /* arbitrary small start */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        end   = 1;                      /* we have computed up to index 1 */
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = end + 1; i <= (size_t)n; ++i) {
        seq_t v1 = (i >= 1) ? cache[i - 1] : 0;
        seq_t v2 = (i >= 2) ? cache[i - 2] : 0;
        seq_t v3 = (i >= 9) ? cache[i - 9] : 0;
        seq_t v4 = (i >= 11) ? cache[i - 11] : 0;
        cache[i] = v1 + v2 - v3 + v4;
    }
    end = (size_t)n;

    return cache[(size_t)n];
}
