/*
 * A207257  Number of 6 X n 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (for n > 7), a(1)=26, a(2)=676, a(3)=4238,
 *            a(4)=15652, a(5)=43498, a(6)=101036, a(7)=207298
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static const seq_t base[] = {0, 26, 676, 4238, 15652, 43498, 101036, 207298};

seq_t a(idx_t n)
{
    /* Handle requests below the offset (undefined behavior, but return 0) */
    if (n < 1)
        return 0;

    /* Static cache for memoization: indices 0..cache_max are computed */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_max = 0;

    /* Expand the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;                     /* capacity for indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                                 /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        if (m <= 7) {                    /* initial values */
            cache[m] = base[m];
            continue;
        }
        /* recurrence: a(m) = 7*a(m-1) - 21*a(m-2) + 35*a(m-3)
                     - 35*a(m-4) + 21*a(m-5) - 7*a(m-6) + a(m-7) */
        cache[m] = 7 * cache[m-1]
                 - 21 * cache[m-2]
                 + 35 * cache[m-3]
                 - 35 * cache[m-4]
                 + 21 * cache[m-5]
                 - 7  * cache[m-6]
                 +   cache[m-7];
    }

    cache_max = n;
    return cache[n];
}
