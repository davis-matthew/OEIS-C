/*
 * A121638  Number of deco polyominoes of height n, having no 2-cell columns.
 *          A deco polyomino is a directed column-convex polyomino in which
 *          the height, measured along the diagonal, is attained only in the
 *          last column.
 * https://oeis.org/A121638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1)+a(n-3) for n>=3; a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n==0 in this sequence */

    /* static cache for memoization */
    static seq_t *cache = NULL;   /* cache[1] holds a(1), etc. */
    static size_t cap = 0;        /* capacity of cache array (in elements) */

    size_t idx = (size_t)n;       /* index within the cache */

    /* grow cache if necessary */
    if (idx >= cap) {
        size_t new_cap = cap > 0 ? cap << 1 : 4;   /* double or start from 4 */
        while (new_cap <= idx) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise new entries to zero (meaning "not computed") */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* compute values up to requested index if not already done */
    for (size_t i = 1; i <= idx; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        if (i == 1 || i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)2;
        } else {
            seq_t term = ((seq_t)(i - 1)) * cache[i - 1] + cache[i - 3];
            cache[i] = term;
        }
    }

    return cache[idx];
}
