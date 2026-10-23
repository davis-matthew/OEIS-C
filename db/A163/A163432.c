/*
 * A163432  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 10*a(n-2) + 10*a(n-3) + 10*a(n-4) - 55*a(n-5)
 *            (n ≥ 6), with initial values: a(0) = 1, a(1) = 12, a(2) = 132,
 *            a(3) = 1452, a(4) = 15972, a(5) = 175626 (The values for n < 6
 *            are taken from the given sequence.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;      /* array of computed terms   */
    static size_t cache_cap = 0;     /* actual allocated size     */
    static idx_t  cache_len = 0;     /* number of terms stored    */

    /* initialise cache with the first six terms if not already done */
    if (!cache) {
        /* allocate at least capacity for 6 elements */
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* set initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)132;
        cache[3] = (seq_t)1452;
        cache[4] = (seq_t)15972;
        cache[5] = (seq_t)175626;
        cache_len = 6;   /* indices 0..5 are stored */
    }

    /* extend the cache if the requested index is beyond what we have */
    if (n >= cache_len) {
        idx_t prev_len = cache_len;
        idx_t new_len = n + 1;

        /* grow the capacity if necessary */
        if (new_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < (size_t)new_len)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – return 0 as a graceful fallback */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = prev_len; i <= n; ++i) {
            if (i < 6) {
                /* the initial values are already set – skip */
                continue;
            }
            /* compute a(i) = 10*a(i-1)+10*a(i-2)+10*a(i-3)+10*a(i-4)-55*a(i-5) */
            seq_t val = (seq_t)10 * cache[i-1];
            val += (seq_t)10 * cache[i-2];
            val += (seq_t)10 * cache[i-3];
            val += (seq_t)10 * cache[i-4];
            val -= (seq_t)55 * cache[i-5];
            cache[i] = val;
        }

        cache_len = n + 1;
    }

    return cache[n];
}
