/*
 * A235303  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=70, a(2)=210, a(3)=518, a(4)=1554, a(5)=4006, a(n) =
 *            3*a(n-1) + 13*a(n-2) - 39*a(n-3) - 40*a(n-4) + 120*a(n-5) for
 *            n>=6
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index computed (>=5 after init) */
    static size_t cache_cap = 0;   /* allocated capacity (unused count) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                                /* start with some spare space */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;                          /* allocation failure */

        /* store the first five terms; index 0 unused */
        cache[1] = (seq_t)70;
        cache[2] = (seq_t)210;
        cache[3] = (seq_t)518;
        cache[4] = (seq_t)1554;
        cache[5] = (seq_t)4006;

        cache_len = 5;
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if ((size_t)n > cache_len) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            /* recurrence: i >= 6, so all terms exist */
            seq_t val = cache[i-1] * (seq_t)3
                      + cache[i-2] * (seq_t)13
                      - cache[i-3] * (seq_t)39
                      - cache[i-4] * (seq_t)40
                      + cache[i-5] * (seq_t)120;
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
