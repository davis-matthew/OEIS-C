/*
 * A019443  Expansion of 1/((1-4x)(1-6x)(1-9x)).
 * https://oeis.org/A019443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 114*a(n-2) + 216*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with initial values: a(0)=1, a(1)=19, a(2)=247           */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return base cases immediately if n < 3. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)247;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t last = (idx_t)-1;          /* last computed index */

    /* First allocation (ensure we store at least up to index n and 2) */
    if (cache == NULL) {
        size_t cap = (size_t)((n > 2) ? n + 1 : 3);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)247;
        last = 2;
    }
    else if (n > last) {
        /* Grow the cache if necessary */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > (size_t)(last + 1)) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) /* allocation failed, abort gracefully */
                return (seq_t)0;
            cache = tmp;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)19 * cache[i - 1]
                 - (seq_t)114 * cache[i - 2]
                 + (seq_t)216 * cache[i - 3];
    }
    last = n;
    return cache[n];
}
