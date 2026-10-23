/*
 * A121752  Number of columns ending at an odd level in all deco polyominoes
 *          of height n. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-3)a(n-1) - (n-1)(n-3)a(n-2) + (n-2)! * (n*⌊n/2⌋ -
 *            (n-2)*⌊(n-1)/2⌋ - 1) with a(1)=1, a(2)=2. This implementation
 *            memoises values and builds the factorial (n-2)! incrementally
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_upto = 0;

    /* static storage of (computed_upto-2)! */
    static seq_t fact = 1;   /* for n=2, (2-2)! = 0! = 1 */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 64;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;                       /* unused (offset 1) */
        cache[1] = (seq_t)1;                /* a(1) */
        cache[2] = (seq_t)2;                /* a(2) */
        computed_upto = 2;
    }

    /* if already computed, return it */
    if (n <= computed_upto)
        return cache[n];

    /* extend cache capacity if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* incremental computation */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* update factorial: increment from (i-1-2)! to (i-2)! */
        fact = fact * (seq_t)(i - 2);

        /* term: (n-2)! * (n*floor(n/2) - (n-2)*floor((n-1)/2) - 1) */
        seq_t part1 = (seq_t)i * (seq_t)(i / 2);
        seq_t part2 = (seq_t)(i - 2) * (seq_t)((i - 1) / 2);
        seq_t term   = fact * (part1 - part2 - (seq_t)1);

        /* recurrence */
        seq_t val = (seq_t)(2 * i - 3) * cache[i - 1]
                  - (seq_t)(i - 1) * (seq_t)(i - 3) * cache[i - 2]
                  + term;

        cache[i] = val;
    }
    computed_upto = n;
    return cache[n];
}
