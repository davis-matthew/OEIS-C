/*
 * A061515  a(0) = 1; a(n) is obtained by incrementing each digit of a(n-1) by
 *          3.
 * https://oeis.org/A061515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   inc_by3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = inc_by3(a(n‑1)) for n > 0 */
#include <stdlib.h>
#include "../utils/inc_by3.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t upto  = 0;          /* number of stored terms (0 .. upto‑1) */
    static size_t cap   = 0;         /* allocated capacity */

    /* If the requested value is already cached, return it */
    if (n < upto)
        return cache[n];

    /* Grow the cache if necessary (doubling strategy) */
    size_t new_cap = (cap == 0) ? 1 : cap;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* allocation failure */
    cache = tmp;
    cap   = new_cap;

    /* Initialize the base case if this is the first call */
    if (upto == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        upto = 1;
    }

    /* Compute missing terms up to the requested index */
    for (idx_t i = upto; i <= n; ++i)
        cache[i] = inc_by3(cache[i - 1]);

    upto = n + 1;
    return cache[n];
}
