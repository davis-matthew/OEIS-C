/*
 * A096297  a(1) = 3, a(n) = a(n-1) + 4*(a(n-1)-floor(a(n-1)^(1/3))^3).
 * https://oeis.org/A096297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3; a(n) = a(n-1) + 4*(a(n-1) - floor(a(n-1)^(1/3))^3), n
 *            >= 2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined below offset */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;           /* allocated slots (including index 0)   */
    static idx_t  cache_len  = 0;           /* highest index computed so far (>=1) */

    /* initialise cache to hold indices up to at least 16 */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = 3;                       /* a(1) = 3 */
        cache_len = 1;
    }

    /* enlarge cache if we need more space */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – leave cache unchanged */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t r    = floor_cube_root(prev);   /* floor(cuberoot(prev)) */
            seq_t c    = r * r * r;                /* r^3 */
            cache[i] = prev + 4 * (prev - c);
        }
        cache_len = n;
    }

    return cache[n];
}
