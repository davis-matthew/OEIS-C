/*
 * A233197  Number of n X 3 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, antidiagonally or
 *          vertically, top left element zero, and 1 appearing before 2 3 4 5
 *          and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colorings with n...
 * https://oeis.org/A233197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=36, a(3)=1440, a(4)=62720;
 *            a(n)=64*a(n-1)-960*a(n-2)+4096*a(n-3) for n>4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of sequence A233197. For n<1, returns 0. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms (1‑based index in 0‑based array) */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* highest index computed (1‑based) */
    static idx_t cap = 0;        /* allocated capacity */

    if (n < 1)
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL)
    {
        /* allocate at least 4 slots for the known initial terms */
        cap = (n < 4 ? 4 : n);
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)1440;
        cache[3] = (seq_t)62720;
        computed = 4;
    }

    /* if we already have the requested value, return it */
    if (n <= computed)
        return cache[n - 1];

    /* grow capacity if needed */
    if (cap < n)
    {
        idx_t new_cap = cap ? cap : 4;
        while (new_cap < n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = computed; i < n; ++i)
    {
        seq_t term = (seq_t)64  * cache[i - 1]
                   - (seq_t)960 * cache[i - 2]
                   + (seq_t)4096 * cache[i - 3];
        cache[i] = term;
    }
    computed = n;
    return cache[n - 1];
}
