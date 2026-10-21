/*
 * A211322  Number of (n+1) X (n+1) -3..3 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or two distinct values.
 * https://oeis.org/A211322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - a(n-3)
 * Initial values: a(1)=11, a(2)=15, a(3)=21
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* unused, offset starts at 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;       /* capacity of cache (number of slots)   */
    static size_t len   = 0;       /* highest index stored in cache (>=1)   */

    /* enlarge the cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;          /* indices 0 .. n   */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                            /* memory exhausted */
        cache = tmp;
        cap   = new_cap;
        /* no need to initialise here – it will be done below */
    }

    /* compute missing terms up to n */
    size_t i = len + 1;                     /* next index to compute */
    if (i <= 3) {                           /* fill base values if missing */
        for (; i <= 3 && i <= (size_t)n; ++i) {
            if (i == 1) cache[1] = 11;
            else if (i == 2) cache[2] = 15;
            else if (i == 3) cache[3] = 21;
        }
        len = (size_t) (i - 1);             /* update highest computed index */
    }

    if (len < 4) {                          /* at least indices 1..3 are known */
        len = (size_t) (i - 1);
        i = len + 1;
    }

    for (; i <= (size_t)n; ++i) {            /* recurrence for i >= 4 */
        cache[i] = (seq_t)2 * cache[i - 1] - cache[i - 3];
    }
    len = (size_t)n;

    return cache[n];
}
