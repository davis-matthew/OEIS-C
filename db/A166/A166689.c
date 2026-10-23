/*
 * A166689  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n ≥ 1: Let c(n) = 1 (n = 0), 2 (1 ≤ n ≤ 11), 1 (n =
 *            12), 0 (n ≥ 13). a(n) = c(n) + 35·∑_{k=1}^{min(n,11)} a(n−k) −
 *            (n ≥ 12 ? 630·a(n−12) : 0) This is equivalent to the recurrence
 *            from the rational generating function: a(n) =
 *            35·a(n−1)+…+35·a(n−11)−630·a(n−12) (n ≥ 12) with a(0)…a(11)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* dynamic cache of computed values */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of values computed (next free index) */

static size_t grow_cache(size_t need)
{
    if (cache_cap >= need)
        return cache_cap;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
    return new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache is large enough to hold a[0..n] */
    grow_cache((size_t)n + 1);

    /* Compute values up to n if not already done */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (i <= 11)
        {
            seq_t sum = 0;
            for (idx_t j = 1; j <= i; ++j)
                sum += cache[(size_t)(i - j)];
            cache[(size_t)i] = (seq_t)2 + (seq_t)35 * sum;
        }
        else if (i == 12)
        {
            /* sum of a[1..11] */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 11; ++j)
                sum += cache[(size_t)(12 - j)];
            cache[12] = (seq_t)1 + (seq_t)35 * sum - (seq_t)630 * cache[0];
        }
        else /* i > 12 */
        {
            /* Maintain sliding window sum of the previous 11 terms. */
            static seq_t window_sum = 0; /* holds sum of a[i-1]..a[i-11] for current i */
            if (i == 13) /* first time we can set window_sum for i=13 */
            {
                /* sum of a[2..12] */
                for (idx_t j = 1; j <= 11; ++j)
                    window_sum += cache[(size_t)(i - j)];
            }
            else if (i > 13)
            {
                /* update window_sum: add new term a[i-1], remove a[i-12] */
                window_sum += cache[(size_t)(i - 1)];
                window_sum -= cache[(size_t)(i - 12)];
            }
            cache[(size_t)i] = (seq_t)35 * window_sum - (seq_t)630 * cache[(size_t)(i - 12)];
        }
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
