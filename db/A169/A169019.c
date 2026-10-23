/*
 * A169019  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=30, … and for n ≥ 0 a(n) = p(n) +
 *            4·(a(n‑1)+…+a(n‑23)) + 10·a(n‑24) (a(n‑24) defined only for
 *            n≥24) where p(0)=1, p(1)…p(23)=2, p(24)=1, p(n)=0 for n>24 Thus
 *            for 1≤n≤23 a(n) = p(n) + 4·∑_{j=0}^{n‑1} a(j) for n=24
 *            a(24)=p(24)+4·∑_{j=1}^{23} a(j)+10·a(0) for n>24
 *            a(n)=4·∑_{j=n‑...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;       /* number of stored terms (>=1) */
    static seq_t  sum_all_prev = 0;     /* sum of all computed terms up to last index */
    static seq_t  sum_last23   = 0;     /* sum of the last 23 terms (valid after n≥24) */

    /* Ensure cache has at least one element a(0). */
    if (cache_size == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 1;                     /* a(0)=1 */
        cache_size = 1;
        sum_all_prev = 1;                 /* sum of a(0) */
    }

    /* If requested term is already computed, return it. */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate up to n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;
    cache = new_cache;

    /* Compute new terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t pi;
        if (i == 0)
            pi = 1;
        else if (i <= 23)
            pi = 2;
        else if (i == 24)
            pi = 1;
        else
            pi = 0;

        /* Compute a(i) using the recurrence. */
        seq_t ai;
        if (i <= 23)
        {
            /* a(i) = p(i) + 4 * sum_{j=0}^{i-1} a(j) */
            ai = pi + (seq_t)4 * sum_all_prev;
        }
        else if (i == 24)
        {
            /* For n=24: use sum of a(1)..a(23) which is sum_all_prev - a0 */
            seq_t sum23 = sum_all_prev - cache[0];
            ai = pi + (seq_t)4 * sum23 + (seq_t)10 * cache[0];
            /* Prepare sum_last23 for the next step: sum of a(2)..a(24) */
            sum_last23 = sum23 + ai - cache[1];
        }
        else /* i > 24 */
        {
            /* a(i) = 4 * sum_last23 + 10 * a(i-24) */
            ai = (seq_t)4 * sum_last23 + (seq_t)10 * cache[i - 24];
            /* Update window for next iteration: add ai, remove a(i-23) */
            sum_last23 += ai;
            sum_last23 -= cache[i - 23];
        }

        cache[i] = ai;

        /* Update total sum for future early terms (up to n=23). */
        if (i <= 23)
            sum_all_prev += ai;
    }

    cache_size = n + 1;
    return cache[n];
}
