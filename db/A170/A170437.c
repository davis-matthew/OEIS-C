/*
 * A170437  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = coef(n) + 42*∑_{k=1}^{43}a(n-k) – 903*a(n-44),
   where coef(0)=1, coef(n)=2 for 1≤n≤43, coef(44)=1 and a(k)=0 for k<0. */

#include <stdlib.h>

/* Static cache to allow arbitrary order of calls */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;

seq_t a(idx_t n)
{
    /* If value already computed, return it */
    if (seq_cache && (size_t)n < seq_cache_len)
        return seq_cache[(size_t)n];

    /* Size needed */
    size_t needed = (size_t)n + 1;

    /* Allocate or grow cache */
    seq_cache = (seq_cache == NULL)
        ? (seq_t *)malloc(needed * sizeof(seq_t))
        : (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));

    /* Compute terms from the current length up to n */
    for (idx_t i = seq_cache_len; i <= n; ++i)
    {
        /* Coefficient from generating function numerator */
        seq_t coef;
        if (i == 0u || i == 44u)
            coef = 1;
        else
            coef = 2;

        /* Sum of last 43 terms (a[i-1] .. a[i-43]) */
        seq_t sum = 0;
        for (int k = 1; k <= 43; ++k)
        {
            if (i >= k)
                sum += seq_cache[(size_t)(i - k)];
        }

        /* Compute a[i] */
        seq_t term = coef;
        term += 42 * sum;
        if (i >= 44)
            term -= 903 * seq_cache[(size_t)(i - 44)];

        seq_cache[(size_t)i] = term;
    }

    /* Update cache size */
    seq_cache_len = needed;

    return seq_cache[(size_t)n];
}
