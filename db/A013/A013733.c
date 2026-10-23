/*
 * A013733  a(n) = 3^(3n+2).
 * https://oeis.org/A013733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(3n+2) * Recurrence: a(0) = 9, a(n) = 27 * a(n-1) (n >
 *            0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* pointer to stored terms      */
    static size_t sz   = 0;         /* number of terms stored       */
    static size_t cap  = 0;         /* capacity of the allocated array */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = alloc_seq(1);
        if (cache == NULL)          /* allocation failure */
            return 0;
        cache[0] = 9;               /* a(0) = 3^2 = 9 */
        sz   = 1;
        cap  = 1;
    }

    /* Return cached value if already computed */
    if ((size_t)n < sz)
        return cache[(size_t)n];

    /* Grow the array if necessary */
    if ((size_t)n + 1 > cap)
        grow_seq_array(&cache, &cap, (size_t)n + 1);

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)sz; i <= n; ++i)
        cache[i] = cache[i - 1] * 27;

    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
