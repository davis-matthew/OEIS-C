/*
 * A177182  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=7, k=-1 and l=-1.
 * https://oeis.org/A177182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{p=0}^{n-1} a(p)*a(n-1-p) - (n+1), a(0) = 1, a(1) = 7
 *            (derived from a(n+1) = Σ_{p=0}^n (a(p)*a(n-p)+k) + l with k = -1
 *            and l = -1, giving the above recurrence.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements stored (>= n+1 when ready) */

    /* allocate or grow the cache if necessary */
    if (cache == NULL) {
        /* first call: ensure space for at least n+1 elements */
        size_t init_size = (size_t)n + 1;
        if (init_size < 2) init_size = 2;       /* we need at least a(0) and a(1) */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_size = 2;
    } else if (n + 1 > cache_size) {
        /* grow to accommodate index n */
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* compute missing terms from cache_size up to n */
        for (idx_t k = cache_size; k <= n; ++k) {
            /* sum of products a(p)*a(k-1-p) for p = 0..k-1 */
            seq_t sum = (seq_t)0;
            for (idx_t p = 0; p < k; ++p) {
                sum += cache[p] * cache[k - 1 - p];
            }
            /* subtract (k+1) as per recurrence */
            sum -= (seq_t)(k + 1);
            cache[k] = sum;
        }
        cache_size = (idx_t)n + 1;
    }

    return cache[n];
}
