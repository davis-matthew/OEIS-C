/*
 * A181324  G.f. 1/(1-sum(n=1,N,x^(n*(3*n-1)/2))).
 * https://oeis.org/A181324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 a(n)=Σ a(n‑p) for all positive pentagonal numbers p≤n
 *            where p = k(3k−1)/2 , k ≥ 1
 * Range:     u8 n <= 21, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;   /* number of elements currently stored */

    /* Grow the cache if necessary */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;                /* size after enlargement */
        seq_t *tmp = (seq_t *)realloc(cache,
                                       (size_t)new_sz * sizeof(seq_t));
        if (!tmp)              /* out of memory – return 0 (safe) */
            return (seq_t)0;
        cache = tmp;

        /* Initialize from the previous size up to the new size */
        if (cache_sz == 0)          /* first call – a(0) = 1 */
            cache[0] = (seq_t)1;

        for (idx_t i = cache_sz; i < new_sz; ++i) {
            if (i == 0)
                continue;                 /* a(0) already set */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; ; ++k) {
                idx_t pent = k * (3 * k - 1) / 2;    /* positive pentagonal */
                if (pent > i) break;
                sum += cache[i - pent];
            }
            cache[i] = sum;
        }
        cache_sz = new_sz;
    }
    return cache[(size_t)n];
}
