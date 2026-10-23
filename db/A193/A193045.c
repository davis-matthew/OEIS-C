/*
 * A193045  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=5*a(n-1)-9*a(n-2)+6*a(n-3)+a(n-4)-3*a(n-5)+a(n-6) with
 *            initial values
 *            a(0)=0,a(1)=1,a(2)=1,a(3)=3,a(4)=8,a(5)=21,a(6)=49 The
 *            recurrence holds for all n≥7. This implementation keeps a static
 *            cache so that calls in any order run in O(n) time
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of valid elements currently stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Grow the cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;   /* avoid overflow */
        /* realloc works for both C and C++ once we cast properly */
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache,
                            new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache,
                            new_cap * sizeof(seq_t));
#endif
        if (!tmp) exit(1);                     /* allocation failed */
        cache   = tmp;
        cache_cap = new_cap;
        /* initialise the new slots to zero (not strictly required) */
        for (size_t i = cache_len; i < cache_cap; ++i) cache[i] = (seq_t)0;
    }

    /* Compute from the first uncomputed index up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0)          cache[i] = (seq_t)0;
        else if (i == 1)     cache[i] = (seq_t)1;
        else if (i == 2)     cache[i] = (seq_t)1;
        else if (i == 3)     cache[i] = (seq_t)3;
        else if (i == 4)     cache[i] = (seq_t)8;
        else if (i == 5)     cache[i] = (seq_t)21;
        else if (i == 6)     cache[i] = (seq_t)49;
        else {   /* recurrence for i >= 7 */
            const seq_t a1 = cache[i-1];
            const seq_t a2 = cache[i-2];
            const seq_t a3 = cache[i-3];
            const seq_t a4 = cache[i-4];
            const seq_t a5 = cache[i-5];
            const seq_t a6 = cache[i-6];

            seq_t term = (seq_t)5 * a1;
            term -= (seq_t)9 * a2;
            term += (seq_t)6 * a3;
            term += a4;
            term -= (seq_t)3 * a5;
            term += a6;

            cache[i] = term;
        }
    }
    cache_len = (size_t)n + 1;   /* all indices up to n are now valid */

    return cache[(size_t)n];
}
