/*
 * A088016  To obtain a(n+1), add the square of the n-th partial sum to the
 *          n-th partial sum of the squares, then divide this result by a(n),
 *          for all n >= 0, with a(0)=1.
 * https://oeis.org/A088016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=1,a(2)=6,a(3)=17; a(n)=3*a(n-1)+a(n-2)-a(n-3) for
 *            n>=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of allocated slots (>=4 once init) */
    static idx_t last_computed = (idx_t)-1;  /* last n for which cache was filled */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_cap = 4;  /* need slots for indices 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* malloc failed; caller should handle */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)17;
        last_computed = 3;
    }

    /* extend cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if they haven't been computed already */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 3*a(i-1) + a(i-2) - a(i-3) */
            cache[i] = (seq_t)((seq_t)3 * cache[i-1] + cache[i-2] - cache[i-3]);
        }
        last_computed = n;
    }

    return cache[n];
}
