/*
 * A228684  Number of 3 X n binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 5*a(n-3) with initial values
 *            a(1)=8, a(2)=17, a(3)=77
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is outside the OEIS offset – return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache – index 0 is unused */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;   /* current allocated size */
    static idx_t   cache_max = 0;   /* largest n whose value is cached */

    /* Grow the cache to hold up to index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)               /* allocation failed – terminate */
            abort();
        cache     = tmp;
        cache_cap = needed;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)8;
        } else if (i == 2) {
            cache[2] = (seq_t)17;
        } else if (i == 3) {
            cache[3] = (seq_t)77;
        } else {
            /* recurrence: 2*a(i-1) + 6*a(i-2) - 5*a(i-3) */
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            cache[i] = (seq_t)(2 * v1 + 6 * v2 - 5 * v3);
        }
    }
    cache_max = n;

    return cache[n];
}
