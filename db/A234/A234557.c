/*
 * A234557  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 18*a(n-2) - 54*a(n-3) - 80*a(n-4) +
 *            240*a(n-5); Initial values: a(1) = 70, a(2) = 220, a(3) = 618,
 *            a(4) = 1954, a(5) = 5506
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The integer types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. For safety, return 0 for lower indices. */
    if (n == 0)
        return 0;

    /* Static cache for memoisation.  Index 0 is unused.  */
    static seq_t *cache = NULL;          /* array of a(1)...a(n) */
    static idx_t cache_last = 0;         /* highest index computed */
    static idx_t cache_cap  = 0;         /* allocated capacity in indices (size) */

    /* Grow the cache if necessary. */
    if (n > cache_last) {
        idx_t new_cap = n + 1;           /* +1 because we ignore index 0 */
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (tmp == NULL) {          /* allocation failure: clear and return 0 */
                free(cache);
                cache = NULL;
                cache_cap = cache_last = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute all missing terms up to n. */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t val = 0;
            switch (i) {
                case 1: val = 70;                break;
                case 2: val = 220;               break;
                case 3: val = 618;               break;
                case 4: val = 1954;              break;
                case 5: val = 5506;              break;
                default:
                    /* Compute with ordering that avoids underflow for unsigned types. */
                    val = cache[i-1] * (seq_t)3
                        + cache[i-2] * (seq_t)18
                        + cache[i-5] * (seq_t)240;
                    val = val - cache[i-3] * (seq_t)54
                        - cache[i-4] * (seq_t)80;
                    break;
            }
            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[n];
}
