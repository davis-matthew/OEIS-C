/*
 * A014898  a(1)=1, a(n) = 15*a(n-1) + n.
 * https://oeis.org/A014898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 15*a(n-1) + n, for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoization to compute values in O(n) time */
/* and O(max_n) space, supporting arbitrary-precision seq_t types. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms, 1-indexed. */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;   /* highest index currently cached */

    /* Grow cache if necessary. */
    if (n > cache_n) {
        idx_t new_n = (cache_n == 0 ? n + 1 : (n + 1 > cache_n * 2 ? n + 1 : cache_n * 2));
        seq_t *tmp = (seq_t *)realloc(cache, (new_n + 1) * sizeof(seq_t));
        cache = tmp;

        /* Compute missing terms from cache_n+1 up to n. */
        idx_t start = (cache_n == 0 ? 1 : cache_n + 1);
        for (idx_t i = start; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)1;
            else
                cache[i] = cache[i - 1] * (seq_t)15 + (seq_t)i;
        }
        cache_n = n;
    }

    return cache[n];
}
