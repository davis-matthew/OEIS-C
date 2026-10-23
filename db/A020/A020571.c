/*
 * A020571  Expansion of g.f. 1/((1-6*x)*(1-7*x)*(1-9*x)).
 * https://oeis.org/A020571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*6^n - 49*7^n/2 + 27*9^n/2 Recurrence: a(0)=1, a(1)=22,
 *            a(2)=325, a(n)=22*a(n-1)-159*a(n-2)+378*a(n-3) (n>2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;   /* pointer to stored terms         */
    static size_t cache_len  = 0;      /* number of terms currently stored */
    static size_t cache_cap  = 0;      /* malloced capacity                */

    size_t target = (size_t)n;         /* cast for indexing convenience   */

    /* initialise cache if first call */
    if (!cache) {
        /* at least three items: 0,1,2 are defined */
        cache_cap = (target < 3 ? 3 : target + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)  /* allocation failure – return zero as safe sentinel */
            return (seq_t)0;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)325;
        cache_len = 3;

        /* if caller requested a lower index, just return it */
        if (target < cache_len)
            return cache[target];
    }

    /* extend cache if necessary */
    if (cache_len <= target) {
        size_t new_len = target + 1;
        if (new_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 3;
            if (new_cap < new_len) new_cap = new_len;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {              /* realloc failed – free and return zero */
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (size_t k = cache_len; k <= target; ++k) {
            if (k == 0)          cache[0] = (seq_t)1;
            else if (k == 1)     cache[1] = (seq_t)22;
            else if (k == 2)     cache[2] = (seq_t)325;
            else {
                seq_t a1 = cache[k-1];
                seq_t a2 = cache[k-2];
                seq_t a3 = cache[k-3];
                cache[k] = (seq_t)( (seq_t)22 * a1
                                   - (seq_t)159 * a2
                                   + (seq_t)378 * a3 );
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
