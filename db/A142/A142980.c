/*
 * A142980  a(1) = 1, a(2) = 5, a(n+2) = 5*a(n+1) + (n + 1)^2*a(n).
 * https://oeis.org/A142980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A142980
   a(1) = 1, a(2) = 5,
   a(n+2) = 5*a(n+1) + (n+1)^2*a(n)   (n ≥ 1)   */

/* seq_t and idx_t are provided by the build system.  The implementation
   below stores previously computed values in a dynamic cache so that
   calls with increasing or arbitrary n remain O(n) and use only the
   required amount of memory.  It works in both C11 and C++17.  */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

/* static cache for memoisation – NULL until first use */
static seq_t *cache         = NULL;   /* array of values, index 0 unused       */
static idx_t  cache_last    = 0;      /* largest n already computed            */
static idx_t  cache_size    = 0;      /* allocated size (in elements)          */

/* ensure that cache holds values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_last)
        return;                     /* already computed */

    /* --- allocation / reallocation ---------------------------------- */
    idx_t new_size = n + 1;          /* include index 0 */
    if (cache == NULL) {
        /* first allocation */
#ifdef __cplusplus
        cache = new seq_t[new_size];
#else
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (!cache)
            abort();                /* out of memory */
        cache_size = new_size;
    } else if (new_size > cache_size) {
        /* grow the existing cache */
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_size];
        /* copy old values */
        for (idx_t i = 0; i <= cache_last; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
#endif
        cache_size = new_size;
    }

    /* --- initialise new entries ------------------------------------- */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
        } else if (k == 2) {
            cache[2] = (seq_t)5;
        } else {
            seq_t km1  = (seq_t)(k - 1);        /* (k‑1) as seq_t */
            seq_t term = km1 * km1 * cache[k - 2];
            cache[k] = (seq_t)5 * cache[k - 1] + term;
        }
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset starts at 1 – n==0 is unused */
    ensure_cache(n);
    return cache[n];
}
