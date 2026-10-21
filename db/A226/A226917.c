/*
 * A226917  Number of non-sphere-homeomorphic crossing optimal 2-page book
 *          drawings of complete graph K_{2n+1}.
 * https://oeis.org/A226917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 8*a(n-4); Initial
 *            values: a(2)=1, a(3)=4, a(4)=9, a(5)=25
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(n) undefined for n < 2; return 0 for such cases */
    if (n < 2)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of allocated slots (in elements) */
    static idx_t max_computed = 0;/* highest index already computed */

    /* first call – initialise cache with the first known terms */
    if (cache_cap == 0) {
        cache_cap = 6;                 /* indices 0 … 5 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                     /* allocation failed */
            return (seq_t)0;

        /* values for indices 0 and 1 are unused but set to 0 for safety */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;   /* a(2) */
        cache[3] = (seq_t)4;   /* a(3) */
        cache[4] = (seq_t)9;   /* a(4) */
        cache[5] = (seq_t)25;  /* a(5) */
        max_computed = 5;
    }

    /* grow the cache if it does not yet contain the requested index */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap)
            new_cap <<= 1;    /* double until it covers n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing values up to n (unordered calls are handled) */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 4*a(i-1) - 2*a(i-2) - 8*a(i-3) + 8*a(i-4) */
            seq_t left  = (seq_t)4 * cache[i - 1] + (seq_t)8 * cache[i - 4];
            seq_t right = (seq_t)2 * cache[i - 2] + (seq_t)8 * cache[i - 3];
            cache[i] = left - right;
        }
        max_computed = n;
    }

    return cache[n];
}
