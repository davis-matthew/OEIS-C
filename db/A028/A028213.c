/*
 * A028213  Expansion of 1/((1-6x)(1-8x)(1-10x)(1-11x)).
 * https://oeis.org/A028213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028213: Expansion of 1/((1-6x)(1-8x)(1-10x)(1-11x))
 * Recurrence (order 4):
 *   a(n) = 35*a(n-1) - 452*a(n-2) + 2548*a(n-3) - 5280*a(n-4)
 * Initial values: a(0)=1, a(1)=35, a(2)=773, a(3)=13783
 */

#include <stdlib.h> /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;   /* current allocation size   */
    static idx_t cache_len = 0;   /* number of terms stored    */

    /* initialise cache with first four terms */
    if (cache_len == 0) {
        cache_cap   = 4;
        cache_len   = 4;
        cache       = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 1;
        cache[1] = 35;
        cache[2] = 773;
        cache[3] = 13783;
    }

    /* extend cache if needed */
    if (n >= cache_len) {
        /* ensure sufficient allocation */
        if (n >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4) continue; /* already initialised */

            seq_t val = 35     * cache[i-1]
                       - 452  * cache[i-2]
                       + 2548 * cache[i-3]
                       - 5280 * cache[i-4];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
