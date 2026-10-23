/*
 * A177165  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=0 and l=-1.
 * https://oeis.org/A177165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A177165: Sequence defined by a(0)=1, a(1)=8,
   a(n) = sum_{p=0}^{n-1} a(p)*a(n-1-p) - 1   for n>=2   (k=0, l=-1) */

#include <stdlib.h>
#ifdef __cplusplus
# include <cstddef>
# include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache      = NULL;   /* array of computed terms      */
    static idx_t  cache_len  = 0;      /* number of terms stored       */
    static idx_t  cache_cap  = 0;      /* current allocated capacity   */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap  = 4;                 /* start with a few slots       */
        cache_len  = 2;                 /* we will store a0 and a1      */
        /* allocate array */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;    /* allocation failed            */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
    }

    /* grow cache if needed */
    if (n >= (idx_t)cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed    */
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }

    /* if the requested term is already computed, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t p = 0; p <= i - 1; ++p) {
            sum += cache[p] * cache[i - 1 - p];
        }
        cache[i] = sum - (seq_t)1;          /* subtract 1 (l = -1) */
    }
    cache_len = n + 1;

    return cache[n];
}
