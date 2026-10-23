/*
 * A193763  Number of signed permutations of length n avoiding (-2, 1) and (2,
 *          -1).
 * https://oeis.org/A193763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = n! + Σ_{j=0}^{n-1} (n-j)! * a(j)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of factorials */
    static seq_t *fact   = NULL;
    static idx_t  fact_len = 0;

    /* cache of sequence values */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;

    /* ------------------------------------------------------------------ */
    /*  Ensure factorials up to n are known.                              */
    /* ------------------------------------------------------------------ */
    if (fact_len <= n)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* unlikely, but safe */
        fact = tmp;
        for (idx_t i = fact_len; i <= n; ++i)
        {
            if (i == 0)
                fact[0] = (seq_t)1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = n + 1;
    }

    /* ------------------------------------------------------------------ */
    /*  Compute a(0)..a(n) into cache if not already present.             */
    /* ------------------------------------------------------------------ */
    if (cache_len <= n)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[0] = (seq_t)1;
            }
            else
            {
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j)
                {
                    sum += fact[i - j] * cache[j];
                }
                cache[i] = fact[i] + sum;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
