/*
 * A211722  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          three or four distinct values for every i,j,k<=n.
 * https://oeis.org/A211722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 25*a(n-3) + 16*a(n-4) + 46*a(n-5) -
 *            48*a(n-6) - 26*a(n-7) + 36*a(n-8) + 4*a(n-9) - 8*a(n-10) Initial
 *            values (for n = 1..10): 48, 82, 140, 250, 448, 824, 1520, 2852,
 *            5364, 10208 The function keeps a cache of all previously
 *            computed terms, so it works correctly for any...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_terms[10] = {
    48, 82, 140, 250, 448, 824, 1520, 2852, 5364, 10208
};
static seq_t *cache = NULL;        /* cache[0] is unused, cache[1] = a(1) */
static idx_t cache_len = 0;        /* size of allocated cache array */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    /* Determine the maximum index that is already cached */
    idx_t max_cached_idx = (cache_len > 0) ? (cache_len - 1) : 0;

    /* If we need higher indices, ensure the array is large enough */
    if (n > max_cached_idx) {
        /* Resize the cache if necessary */
        if (n + 1 > cache_len) {
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failure */
            cache = tmp;
            cache_len = (size_t)(n + 1);
        }

        /* Compute all missing terms, starting just after the last
           already cached index. */
        for (idx_t i = max_cached_idx + 1; i <= n; ++i) {
            if (i <= 10) {
                /* Base terms */
                cache[i] = base_terms[i - 1];
            } else {
                /* Recurrence for i ≥ 11 */
                seq_t pos =  4 * cache[i - 1]
                           + 2 * cache[i - 2]
                           + 16 * cache[i - 4]
                           + 46 * cache[i - 5]
                           + 36 * cache[i - 8]
                           + 4  * cache[i - 9];

                seq_t neg = 25 * cache[i - 3]
                           + 48 * cache[i - 6]
                           + 26 * cache[i - 7]
                           + 8  * cache[i -10];

                cache[i] = pos - neg;   /* pos >= neg for all valid n */
            }
        }
    }

    return cache[n];
}
