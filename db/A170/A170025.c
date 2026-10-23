/*
 * A170025  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170025
 * Formula: linear recurrence derived from the given generating function
 * a(0) = 1
 * for 1 <= n < 36 :  a(n) = 2 + 14 * sum_{k=0}^{n-1} a(k)
 *                 = coeff + 14 * S(n-1),  coeff=2
 * for n = 36 :       a(36) = 1 + 14 * sum_{k=1}^{35} a(k) - 105 * a(0)
 *                 = coeff + 14 * S(35) - 105 * a(0), coeff=1
 * for n > 36 :       a(n) = 14 * sum_{k=n-35}^{n-1} a(k) - 105 * a(n-36)
 *                 = coeff + 14 * S(n-1) - 105 * a(n-36), coeff=0
 *
 * The implementation stores computed values in a static cache so that
 * repeated calls to a() with arbitrary indices are answered in O(n)
 * time where n is the largest index requested, independent of call order.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Return the nth term of A170025. */
seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_cap  = 0;      /* allocated size */
    static idx_t  last       = (idx_t)-1; /* index of last computed term */
    static seq_t sum35       = 0;      /* sums last up to 35 values */

    /* Ensure the cache is large enough to hold terms up to n */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t term = 0;

        /* Coefficient part of the generating function */
        if (i == 0)          term = 1;
        else if (i < 36)     term = 2;
        else if (i == 36)    term = 1;
        /* for i > 36 term starts at 0 */

        /* Add 14 * sum of last up to 35 terms (if any) */
        if (i > 0) term += (seq_t)14 * sum35;

        /* Subtract 105 * a(i-36) for i >= 36 */
        if (i >= 36) term -= (seq_t)105 * cache[i - 36];

        /* Store the computed term */
        cache[i] = term;

        /* Update sum35 for use in the next iteration:
           sum35 now must equal sum of a(max(0,i-34)) .. a(i) */
        sum35 += term;
        if (i >= 35) sum35 -= cache[i - 35];

        last = i;
    }

    return cache[n];
}
