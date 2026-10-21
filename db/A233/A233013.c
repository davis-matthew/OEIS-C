/*
 * A233013  Number of n X 3 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally or vertically, top left element zero,
 *          and 1 appearing before 2 in row major order.
 * https://oeis.org/A233013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 45*a(n-2) + 27*a(n-3); Initial values:
 *            a(1)=5, a(2)=74, a(3)=1202
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoisation of the recurrence */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* largest index stored in cache (by 1-based index) */

    if (n <= cap)                     /* already computed */
        return cache[n];

    /* Grow the cache to at least index n */
    size_t new_cap = (cap == 0) ? 3 : cap;
    while (new_cap < (size_t)n)
        new_cap <<= 1;                /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();                /* fatal if out of memory */
    cache = tmp;

    /* initialise the first three terms on first allocation */
    if (cap == 0) {
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)74;
        cache[3] = (seq_t)1202;
    }

    /* compute all missing terms using the recurrence */
    for (size_t i = cap + 1; i <= new_cap; ++i) {
        if (i <= 3) continue;        /* skip base terms */
        cache[i] = (seq_t)19 * cache[i - 1]
                 - (seq_t)45 * cache[i - 2]
                 + (seq_t)27 * cache[i - 3];
    }

    cap = new_cap;
    return cache[n];
}
