/*
 * A202789  Number of n X 2 binary arrays with every one adjacent to another
 *          one horizontally, diagonally, antidiagonally or vertically.
 * https://oeis.org/A202789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 6*a(n-3), with a(1)=2, a(2)=12,
 *            a(3)=50
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* very small lists are handled by the recurrence directly */
    if (n <= 3) {
        static const seq_t base[4] = {0, 2, 12, 50};
        return base[n];
    }

    /* cache for previously computed values (1‑based) */
    static seq_t *cache = NULL;      /* cache[1] .. cache[max_n] */
    static idx_t max_n      = 0;     /* maximum index that's computed */

    /* initialise cache on first use */
    if (cache == NULL) {
        /* at least 4 slots, plus possibly more if n is larger */
        idx_t init_size = n > 3 ? n + 1 : 4;
        cache = (seq_t *)calloc((size_t)init_size, sizeof(seq_t));
        /* store the small base values */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)50;
        max_n = 3;
    }

    /* grow the cache if our request exceeds the current size */
    if (n > max_n) {
        /* realloc to n+1 slots (1‑based indexing) */
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        /* compute new terms incrementally */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1] - 2 * cache[i - 2] + 6 * cache[i - 3];
        }
        max_n = n;
    }

    return cache[n];
}
