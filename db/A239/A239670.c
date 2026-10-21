/*
 * A239670  Expansion of 1/((1-x)*(1-81*x)).
 * https://oeis.org/A239670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*a(n-1) + 1 with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;           /* number of allocated terms */

    /* if requested term not yet in cache, grow and compute */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;                 /* include term n */
        seq_t *tmp;

        /* allocate or resize cache */
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (tmp == NULL) return 0;   /* allocation failure: return zero */

        cache = tmp;

        /* compute terms from the existing cache size up to n */
        for (size_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1; /* a(0) */
            } else {
                cache[i] = cache[i - 1] * (seq_t)81 + (seq_t)1;
            }
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
