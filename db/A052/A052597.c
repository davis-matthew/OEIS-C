/*
 * A052597  E.g.f. 1/(1-x^2-x^3).
 * https://oeis.org/A052597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=0, a(2)=2;
   Recurrence: a(n+3) = (n^3+6n^2+11n+6)*a(n)+(n^2+5n+6)*a(n+1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* fast paths for the first three terms */
    if (n < cache_len) return cache[n];
    if (n <= 2) {
        /* initialise cache if still empty */
        if (cache_len == 0) {
            size_t init_cap = 4;
            cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
            if (!cache) { /* allocation failure: abort */ }
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)2;
            cache_len = 3;
            cache_cap = init_cap;
        }
        return cache[n];
    }

    /* allocate or grow storage to hold up to n */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) { /* allocation failure: abort */ }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base case if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache_len = 3;
    }

    /* compute values from current cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* compute a(i) using recurrence with m = i-3 */
        size_t m = i - 3;
        /* intermediate small integers fit in 64 bits; cast to seq_t */
        seq_t term1 = (seq_t)(m*m*m + 6*m*m + 11*m + 6);
        seq_t term2 = (seq_t)(m*m + 5*m + 6);
        seq_t part1 = cache[m] * term1;
        seq_t part2 = cache[m + 1] * term2;
        cache[i] = part1 + part2;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
