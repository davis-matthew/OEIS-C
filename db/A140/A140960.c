/*
 * A140960  a(n) = (2*(-1)^n - 2^(n+1) + 3*n*2^n)/9.
 * https://oeis.org/A140960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*(-1)^n - 2^(n+1) + 3*n*2^n)/9 Recurrence used: a(0)=0,
 *            a(1)=0, a(2)=2; a(n) = 3*a(n-1) - 4*a(n-3) for n >= 3
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;                   /* at least space for indices 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        cache_len = 3;                   /* 0,1,2 stored */
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache if necessary */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {               /* allocation failure – abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms via recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * 3;    /* 3*a(i-1)   */
        seq_t t2 = cache[i - 3] * 4;    /* 4*a(i-3)   */
        cache[i] = t1 - t2;             /* a(i) = t1 - t2 */
    }
    cache_len = n + 1;                    /* update cache length */
    return cache[n];
}
