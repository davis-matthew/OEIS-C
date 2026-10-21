/*
 * A094869  E.g.f.: exp(5x)/(1-5x)^(1/5).
 * https://oeis.org/A094869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (5*n+1)*a(n-1) - 25*(n-1)*a(n-2)
 * Initial values: a(0) = 1, a(1) = 6
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cache_cap   = 0;
    static idx_t  computed_max = 0;

    /* ensure minimal cache size for indices 0 and 1 */
    if (n == 0 || n == 1) {
        if (cache_cap < 2) {
            cache_cap = 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return 0;  /* out of memory; caller will treat result as 0 */
            cache[0] = 1;
            cache[1] = 6;
            computed_max = 1;
        }
        return cache[n];
    }

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache_cap = new_cap;
    }
    /* initialise base entries if this is the first call */
    if (computed_max < 1) {
        cache[0] = 1;
        cache[1] = 6;
        computed_max = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * ((seq_t)(5 * i + 1));
        seq_t term2 = cache[i - 2] * ((seq_t)(25 * (i - 1)));
        cache[i] = term1 - term2;
    }
    computed_max = n;
    return cache[n];
}
