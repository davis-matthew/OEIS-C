/*
 * A121746  Number of deco polyominoes of height n, consisting only of columns
 *          of even length. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)/2)*a(n-1)+floor((n+1)/2)*a(n-2); Initial
 *            values: a(0)=1, a(1)=0
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to memoize computed values */
    static seq_t *cache = NULL;
    static idx_t last = 0;          /* highest index computed so far */
    static size_t cap = 0;           /* allocated capacity of cache */

    /* initialize cache on first call, ensuring space for indices 0 and 1 */
    if (cache == NULL) {
        cap = (n < 1) ? 2 : (size_t)(n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;         /* a(0) */
        cache[1] = (seq_t)0;         /* a(1) */
        last = 1;
    }
    /* grow cache if requested index exceeds current capacity */
    else if (n > (idx_t)(cap - 1)) {
        size_t newcap = (cap * 2 > (size_t)(n + 1)) ? cap * 2 : (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap = newcap;
    }

    /* compute values up to index n if not already done */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t term = ((seq_t)((i - 1) / 2)) * cache[i - 1]
                        + ((seq_t)((i + 1) / 2)) * cache[i - 2];
            cache[i] = term;
        }
        last = n;
    }

    return cache[n];
}
