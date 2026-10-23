/*
 * A115220  Expansion of x^4*(1-2*x-2*x^2)/(1-4*x+x^4-2*x^5-2*x^6).
 * https://oeis.org/A115220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=a(1)=a(2)=a(3)=0, a(4)=1, a(5)=2, a(6)=6;
 * for n>6: a(n)=4*a(n-1)-a(n-4)+2*a(n-5)+2*a(n-6)
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Small indices handled directly */
    if (n <= 3)
        return (seq_t)0;

    /* Static dynamic array to store computed values */
    static seq_t *cache            = NULL;
    static size_t cache_cap        = 0;   /* allocated array capacity */
    static idx_t  max_comp         = 0;   /* highest index computed */
    static int    initialized      = 0;   /* base values set? */

    /* Ensure the array is large enough */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        /* Copy old contents, if any */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* Free the old buffer */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache       = new_cache;
        cache_cap   = new_cap;
    }

    /* Initialise the first seven terms once */
    if (!initialized) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)2;
        cache[6] = (seq_t)6;
        max_comp = 6;
        initialized = 1;
    }

    /* Compute any missing values up to n */
    if (max_comp < (idx_t)n) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* recurrence: a(i) = 4*a(i-1) - a(i-4) + 2*a(i-5) + 2*a(i-6) */
            seq_t val = 4 * cache[i-1] - cache[i-4];
            val += 2 * cache[i-5];
            val += 2 * cache[i-6];
            cache[i] = val;
        }
        max_comp = n;
    }

    return cache[n];
}
