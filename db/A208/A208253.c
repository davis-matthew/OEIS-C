/*
 * A208253  Number of n X 3 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 16*a(n-2) for n>3, a(1)=5, a(2)=96,
 *            a(3)=2040, with a(0) unused
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cap    = 0;   /* allocated capacity (maximum index) */
    static idx_t filled = 0;   /* highest index computed */

    if (n == 0)
        return (seq_t)0;                       /* undefined for n=0 */

    /* first call: allocate space and initialize the seed values */
    if (!cache) {
        /* start with at least space for indices 0 .. 4 */
        cap = 4;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)96;
        cache[3] = (seq_t)2040;
        filled = 3;
    }

    /* if the requested value is already known, return it */
    if (n <= filled)
        return cache[n];

    /* grow the cache up to n */
    while (n > filled) {
        /* ensure we have enough capacity */
        if (filled >= cap) {
            idx_t new_cap = (cap < 4) ? 4 : cap * 2;
            while (new_cap < n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* compute the next value by the recurrence */
        idx_t idx = filled + 1;
        if (idx > 3)
            cache[idx] = (seq_t)22 * cache[idx - 1] - (seq_t)16 * cache[idx - 2];
        /* for idx == 4+ it will always hit the recurrence path */
        filled = idx;
    }

    return cache[n];
}
