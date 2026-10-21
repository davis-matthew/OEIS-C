/*
 * A239040  Number of n X 1 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n - 1) + 2*a(n - 3), with a(1) = 3, a(2) = 7, a(3) =
 *            16 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u)               /* undefined for n < 1, safe to return 0 */
        return (seq_t)0;

    /* static memoisation cache */
    static seq_t *cache  = NULL;
    static size_t  cap   = 0;     /* allocated capacity (highest index that can be addressed) */
    static idx_t   last  = 0;     /* highest n for which cache[n] is valid */

    /* allocate or grow cache to hold index n */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 4) new_cap = 4;            /* need space for base values */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed; abort gracefully */
            free(cache);
            cache = NULL;
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base values if not yet done */
    if (last < 3) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)16;
        last = 3;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + 2*a(i-3) */
        cache[i] = cache[i - 1] + cache[i - 1] + cache[i - 3] + cache[i - 3];
    }
    last = n;
    return cache[n];
}
