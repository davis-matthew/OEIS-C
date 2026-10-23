/*
 * A028493  a(0) = 16, a(n+1) = 3a(n) - (6-n)^2.
 * https://oeis.org/A028493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 16; a(n+1) = 3*a(n) - (6-n)^2 (n ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache ------------------------------------------------------- */
    static seq_t *cache     = NULL;   /* array of computed terms         */
    static idx_t cache_size = 0;      /* number of terms stored          */
    static idx_t cache_cap  = 0;      /* allocated capacity of cache     */

    /* initialise cache with the first term if empty --------------------- */
    if (cache_size == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;                 /* allocation failure          */
        cache[0] = 16;                        /* a(0) */
        cache_size = 1;
    }

    /* grow cache if we need to compute terms up to index n ---------------- */
    if (cache_size <= n) {
        /* increase capacity (doubling strategy) */
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap = new_cap ? new_cap * 2 : 1;   /* never 0 */

        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;                    /* allocation failure      */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms using the recurrence --------------------- */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];

            /* (6 - (i-1))^2  with signed intermediate                */
            long long d = 6 - (long long)(i - 1);
            seq_t sq = (seq_t)(d * d);

            cache[i] = prev * (seq_t)3 - sq;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
