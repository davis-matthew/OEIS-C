/*
 * A202464  Number of (n+2) X 3 binary arrays with no more than two of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2) + 18*a(n-3) - 6*a(n-4) + 2*a(n-5) -
 *            a(n-6) (valid for n ≥ 7) Initial values: a(1)=265, a(2)=1573,
 *            a(3)=9253, a(4)=54085, a(5)=317179, a(6)=1858993
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_comp = 0;

    /* Initialise cache with the first six values (index 0 is unused) */
    if (cache == NULL) {
        cache_size = 7;                /* indices 0 … 6 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)
            return (seq_t)0;          /* allocation failed – return 0 */

        cache[1] = (seq_t)265;
        cache[2] = (seq_t)1573;
        cache[3] = (seq_t)9253;
        cache[4] = (seq_t)54085;
        cache[5] = (seq_t)317179;
        cache[6] = (seq_t)1858993;
        last_comp   = 6;
    }

    /* Grow cache if needed to reach index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL)
            return (seq_t)0;          /* realloc failed – return 0 */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Return value if already computed */
    if (n <= last_comp)
        return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)4
                  + cache[i - 2] * (seq_t)8
                  + cache[i - 3] * (seq_t)18
                  - cache[i - 4] * (seq_t)6
                  + cache[i - 5] * (seq_t)2
                  - cache[i - 6];
        cache[i] = val;
    }
    last_comp = n;

    return cache[n];
}
