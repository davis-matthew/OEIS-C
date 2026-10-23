/*
 * A169200  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169200 – number of reduced words of length n in a Coxeter group
 *  on 43 generators with (S_i)^2 = (S_i S_j)^27 = I.
 *
 *  Formula (recurrence form):
 *      a(0) = 1
 *      for n ≥ 1
 *          a(n) = 41 * (a(n-1)+a(n-2)+...+a(n-26))
 *                 – 861 * a(n-27)
 *                 + p(n)
 *          where
 *              p(n) = 2   (1 ≤ n ≤ 26)
 *              p(n) = 1   (n = 0 or 27)
 *              p(n) = 0   (otherwise)
 *
 *  This implementation uses a static cache, so calls may be in any
 *  order and are O(n) overall.  It works for arbitrary-precision
 *  seq_t as well as fixed-width integer types.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;   /* array of a(i) for i < cache_size */
    static idx_t  cache_size = 0;      /* number of elements currently stored */

    if (n >= cache_size) {
        /* enlarge the cache to index n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed – return zero */
        cache = tmp;

        /* fill in the missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                /* a(0) = 1 */
                val = 1;
            } else {
                /* inhomogeneous part p(i) */
                if (i <= 26)
                    val = 2;
                else if (i == 27)
                    val = 1;
                else
                    val = 0;

                /* 41 * sum of last up to 26 terms */
                idx_t maxj = (i < 26) ? i : 26;
                for (idx_t j = 1; j <= maxj; ++j) {
                    val += ( (seq_t)41 ) * cache[i - j];
                }

                /* –861 * a(i‑27) for i ≥ 27 */
                if (i >= 27) {
                    val -= ( (seq_t)861 ) * cache[i - 27];
                }
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
