/*
 * A130804  a(1) = 1; for n > 1, a(n) = smallest k > a(n-1) such that s+k is
 *          divisible by sum of digits of (s+k), where s = Sum_{j=1..n-1}
 *          a(j).
 * https://oeis.org/A130804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1, a(n) is the smallest integer k>a(n‑1) such that
 *            (S+k) mod digit_sum(S+k) == 0, where S = Σ_{j=1}^{n-1} a(j)
 * Range:     u8 n <= 16, u16 n <= 659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined below offset */

    /* 1‑based cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;
    static seq_t sum_so_far = 0;

    /* initialise on first call */
    if (!cache)
    {
        cache_cap = 4;                                /* arbitrary initial size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                                  /* allocation failure */

        cache[1] = (seq_t)1;                          /* a(1)=1 */
        sum_so_far = cache[1];
        cache_last = 1;
    }

    /* if already computed, just return */
    if (n <= cache_last)
        return cache[n];

    /* grow cache if needed */
    size_t needed = (size_t)n + 1; /* index n needs array size n+1 (1‑based) */
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    seq_t k = cache[cache_last] + 1; /* start just after last known term */
    seq_t last_term = cache[cache_last];
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        /* find smallest k > last_term with required property */
        while (1)
        {
            seq_t cand = sum_so_far + k;
            seq_t ds = digit_sum(cand);
            if (cand % ds == 0)
                break;
            ++k;
        }
        cache[i] = k;
        sum_so_far += k;
        last_term = k;
        k = k + 1;            /* next candidate must be > current term */
        cache_last = i;
    }

    return cache[n];
}
