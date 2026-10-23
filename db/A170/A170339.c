/*
 * A170339  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P_n + 40 * Σ_{k=1}^{min(41,n)} a(n-k) – (n≥42 ?
 *            820·a(n-42) : 0) where P_n = 1 for n=0 or 42, P_n = 2 for
 *            1≤n≤41, and P_n = 0 otherwise. This implements the linear
 *            recurrence obtained from the rational generating function of
 *            A170339
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache keeps all values that have been computed so far */
    static seq_t *cache = NULL;
    static size_t cache_used  = 0;   /* number of valid entries (next index to compute) */
    static size_t cache_cap   = 0;   /* allocated capacity */

    size_t target = (size_t)n;
    if (cache_used <= target)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= target)
            new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        for (size_t i = cache_used; i <= target; ++i)
        {
            if (i == 0)
            {
                cache[i] = 1;
                continue;
            }

            seq_t P;
            if (i == 42)
                P = 1;
            else if (i <= 41)
                P = 2;
            else
                P = 0;

            /* S1 = sum of the last min(41,i) terms a[i-1],...,a[max(0,i-41)] */
            size_t start = (i >= 42) ? (i - 41) : 0;
            seq_t S1 = 0;
            for (size_t j = start; j < i; ++j)
                S1 += cache[j];

            seq_t term = P;
            term += (seq_t)40 * S1;
            if (i >= 42)
                term -= (seq_t)820 * cache[i - 42];

            cache[i] = term;
        }
        cache_used = target + 1;
    }

    return cache[(size_t)n];
}
