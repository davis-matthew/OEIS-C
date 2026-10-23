/*
 * A025920  Expansion of 1/((1-x^8)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A025920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) + a(n-9) + a(n-12) - a(n-17) - a(n-20) - a(n-21) +
 *            a(n-29) * with initial values a(0)=1 and a(n)=0 for 1≤n≤7
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, size_t */
#include <string.h>   /* memset */

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache      = NULL;   /* array of computed a(i) */
    static idx_t cache_size  = 0;      /* current capacity of cache   */
    static idx_t max_computed = 0;     /* highest n for which cache[n] is valid */

    /* first call: allocate and initialise cache up to n */
    if (cache == NULL) {
        cache_size  = (idx_t)(n + 1);             /* cast to idx_t for safety */
        /* allocate raw memory */
#ifdef __cplusplus
        cache = new seq_t[cache_size];           /* calls constructors (C++) */
#else
        cache = (seq_t *)malloc((size_t)(cache_size) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failure */
        memset(cache, 0, (size_t)(cache_size) * sizeof(seq_t)); /* initialise to 0 */
#endif
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* extend cache if requested n exceeds current capacity */
    if (n >= cache_size) {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[(idx_t)(n + 1)];
        /* copy old values */
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        /* default‑construct the new slots, they are already value‑initialised */
        delete [] cache;
        cache = new_cache;
        cache_size = (idx_t)(n + 1);
#else
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* reallocation failure */
        cache = tmp;
        /* initialise newly allocated slots to 0 */
        memset((void *)(cache + cache_size),
               0,
               (size_t)((n + 1 - cache_size)) * sizeof(seq_t));
        cache_size = (idx_t)(n + 1);
#endif
    }

    /* compute missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = (seq_t)0;
            if (i >= 8)  val += cache[i - 8];
            if (i >= 9)  val += cache[i - 9];
            if (i >= 12) val += cache[i - 12];
            if (i >= 17) val -= cache[i - 17];
            if (i >= 20) val -= cache[i - 20];
            if (i >= 21) val -= cache[i - 21];
            if (i >= 29) val += cache[i - 29];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
