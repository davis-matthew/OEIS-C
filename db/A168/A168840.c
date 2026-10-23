/*
 * A168840  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 2 + 17*Σ_{k=0}^{n-1} a(k) for 1 ≤ n < 20 a(n) =
 *            17*Σ_{k=1}^{19} a(n-k) – 153*a(n-20) for n ≥ 20 (derived from
 *            the rational generating function of A168840) This implements an
 *            O(n) recurrence that works for any idx_t n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;         /* array of computed terms       */
    static idx_t cache_size = 0;        /* number of valid entries       */
    static idx_t cache_capacity = 0;    /* allocated size                */
    static seq_t sum19 = 0;             /* rolling sum of last 19 terms   */

    /* initialise/cache first 20 terms on first call */
    if (cache_size == 0)
    {
        /* we need at least 20 entries to start */
        idx_t init_capacity = 20;
        cache = (seq_t *)malloc(sizeof(seq_t) * init_capacity);
        cache_capacity = init_capacity;

        /* compute a(0)..a(19) by the inhomogeneous recurrence */
        seq_t running_sum = 0;          /* Σ_{k=0}^{i-1} a(k)          */
        cache[0] = (seq_t)1;
        running_sum += cache[0];

        for (idx_t i = 1; i <= 19; ++i)
        {
            cache[i] = 2 + 17 * running_sum;
            running_sum += cache[i];
        }

        /* prepare rolling sum of the 19 most recent terms
           (a(1)..a(19) for the first homogeneous step) */
        sum19 = 0;
        for (idx_t i = 1; i <= 19; ++i)
            sum19 += cache[i];

        cache_size = 20;  /* indices 0..19 are valid */
    }

    /* if the requested term is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* ensure capacity for up to n */
    if (cache_capacity <= n)
    {
        idx_t new_capacity = cache_capacity * 2;
        if (new_capacity == 0) new_capacity = 64;
        while (new_capacity <= n)
            new_capacity <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_capacity);
        cache_capacity = new_capacity;
    }

    /* extend cache until we reach n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* compute using the homogeneous recurrence */
        seq_t new_val = 17 * sum19 - 153 * cache[i - 20];
        cache[i] = new_val;

        /* update the rolling sum of the last 19 terms:
           add new_val, remove the term that falls out of the window */
        sum19 += new_val - cache[i - 19];
    }

    cache_size = n + 1;
    return cache[n];
}
