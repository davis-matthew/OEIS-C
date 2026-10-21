/*
 * A059280  Expansion of e.g.f. exp(x*(1-x)/(1-2*x)).
 * https://oeis.org/A059280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} (k+1)·c_{k+1}·(n−1)_k·a(n−1−k) where c_1
 *            = 1 and c_m = 2^{m-2} for m ≥ 2. Here (x)_k denotes the falling
 *            factorial x·(x−1)…(x−k+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
    }

    if (n < cache_size)
        return cache[n];

    /* extend cache up to n */
    idx_t new_cap = n + 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

    for (idx_t i = cache_size; i <= n; ++i) {
        /* compute a(i) using recurrence from a(i-1) down to a(0) */
        idx_t cur_n = i - 1;          /* n in the recurrence */
        seq_t sum = 0;
        seq_t falling = 1;            /* (cur_n)_k for current k */

        for (idx_t k = 0; k <= cur_n; ++k) {
            /* coefficient c_{k+1} */
            seq_t c;
            if (k == 0)
                c = (seq_t)1;
            else
                c = ((seq_t)1) << (k - 1);   /* 2^{k-1} */

            /* term = falling * (k+1) * c * a[cur_n - k] */
            seq_t term = falling;
            term = term * (seq_t)(k + 1);
            term = term * c;
            term = term * cache[cur_n - k];
            sum = sum + term;

            /* update falling for next k: multiply by (cur_n - k) */
            if (k < cur_n)
                falling = falling * (seq_t)(cur_n - k);
        }
        cache[i] = sum;
    }

    cache_size = new_cap;
    return cache[n];
}
