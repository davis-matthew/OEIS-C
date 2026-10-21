/*
 * A066949  Take the sum of the previous two terms, subtract n if this sum is
 *          greater than n.
 * https://oeis.org/A066949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(0)=0, a(1)=1; a(n)=a(n-1)+a(n-2)-n if a(n-1)+a(n-2)>n,
 *            otherwise a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently computed */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t need = 2;                 /* need at least two entries */
        if (n+1 > need) need = n+1;
        cache_cap  = need;
        cache_size = 2;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
#endif
        cache[0] = 0;
        cache[1] = 1;
    }

    /* return early if we already have the answer */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache if necessary */
    size_t needed = n + 1;
    if (needed > cache_cap) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[needed];
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete [] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
        cache = tmp;
#endif
        cache_cap = needed;
    }

    /* compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        if (sum > (seq_t)i)
            cache[i] = sum - (seq_t)i;
        else
            cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
