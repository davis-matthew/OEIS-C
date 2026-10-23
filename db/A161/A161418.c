/*
 * A161418  Number of triangles in the Y-toothpick structure after n rounds.
 * https://oeis.org/A161418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 0 a(4) = 6 For even n ≥ 6:
 *            a(n) = 2 * a(n-2) For odd n ≥ 5: a(n) = a(n-1) + a(n-4) (The
 *            recurrence follows from the self‑similar structure of the
 *            Y‑toothpick fractal.)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    size_t i, up_to;

    /* ensure cache can hold up to n */
    up_to = (size_t)n;
    if (up_to >= cap) {
        size_t new_cap = up_to + 1;          /* store 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure fallback */
        cache = tmp;
        /* initialise new entries with 0 */
        for (i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (i = 0; i <= up_to; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)0;
        } else if (i == 4) {
            cache[i] = (seq_t)6;
        } else if ((i & 1) == 0) {          /* even n ≥ 6 */
            cache[i] = cache[i - 2] + cache[i - 2];
        } else {                            /* odd n ≥ 5 */
            cache[i] = cache[i - 1] + cache[i - 4];
        }
    }

    return cache[up_to];
}
