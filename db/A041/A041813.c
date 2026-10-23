/*
 * A041813  Denominators of continued fraction convergents to sqrt(427).
 * https://oeis.org/A041813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A041813
 *
 *  Denominators of continued fraction convergents to sqrt(427).
 *
 *  Recurrence: a(n) = 124*a(n-4) - a(n-8)   for n > 7
 *  Initial values:
 *      a(0)=1, a(1)=1, a(2)=2, a(3)=3,
 *      a(4)=122, a(5)=125, a(6)=247, a(7)=372
 *
 *  The function uses lazy memoisation; it works for any seq_t type
 *  (integral or arbitrary-precision class) and for any idx_t.
 */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

static const seq_t _init_vals[8] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
    (seq_t)122, (seq_t)125, (seq_t)247, (seq_t)372
};

seq_t a(idx_t n)
{
    /* trivial case – all initial terms are known */
    if (n < 8)
        return _init_vals[n];

    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;     /* allocated length of cache   */
    static idx_t  last_computed = (idx_t)-1;  /* highest n already computed */

    /* allocate cache if this is the first call */
    if (cache == NULL) {
        cache_size = n + 1 > 8 ? n + 1 : 8;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* allocation failure – return 0 */

        /* initialise the first 8 terms */
        memcpy(cache, _init_vals, 8 * sizeof(seq_t));
#ifdef __cplusplus
        for (idx_t i = 8; i < cache_size; ++i)
            new(&cache[i]) seq_t();   /* default‑construct new elements */
#endif
        last_computed = 7;
    }
    /* grow the cache if needed */
    else if (cache_size <= n) {
        idx_t new_size = cache_size * 2;
        if (new_size <= n)
            new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failure */
        cache = tmp;
#ifdef __cplusplus
        for (idx_t i = cache_size; i < new_size; ++i)
            new(&cache[i]) seq_t();   /* construct new elements */
#endif
        cache_size = new_size;
    }

    /* compute any missing terms up to n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i)
            cache[i] = (seq_t)124 * cache[i - 4] - cache[i - 8];
        last_computed = n;
    }

    return cache[n];
}
