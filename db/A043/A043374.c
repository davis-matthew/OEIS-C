/*
 * A043374  Numbers having two 1's in base 6.
 * https://oeis.org/A043374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   count_ones_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generator: a(n) = n-th positive integer that has exactly two '1' digits
   in its base‑6 representation.  The function builds the sequence
   incrementally and caches the results so that calls in any order are
   efficient. */

#include <stdlib.h>
#include "../utils/count_ones_base6.h"

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL; /* cached values, 1‑indexed  (cache[0] == a(1)) */
    static idx_t  csize  = 0;    /* how many values are cached */
    static seq_t  last = 0;     /* last integer tested              */

    if (n == 0)                 /* offset is 1; undefined below 1  */
        return 0;

    if (csize < n) {
        /* Grow cache to hold n entries. */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failed */
        cache = tmp;

        /* Compute new values up to n. */
        while (csize < n) {
            do
                ++last;               /* try next integer */
            while (count_ones_base6(last) != 2);  /* until it has two 1's */

            cache[csize++] = last;    /* store it */
        }
    }
    return cache[n - 1];
}
