/*
 * A082987  a(n) = Sum_{k=0..n} 3^k*F(k) where F(k) is the k-th Fibonacci
 *          number.
 * https://oeis.org/A082987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 3^k * F(k) where F(k) is the k‑th Fibonacci
 *            number with F(0)=0, F(1)=1. Equivalent linear recurrence: a(0) =
 *            0, a(1) = 3, a(2) = 12, a(n) = 4*a(n-1) + 6*a(n-2) - 9*a(n-3)
 *            for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values for previously computed indices. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of valid entries in cache */

    /* If the value is already cached, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Resize cache to hold indices up to n. */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_size);
    if (!cache)
        return 0; /* Allocation failure – return zero as a fallback. */

    /* Compute missing values using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)3;
        else if (i == 2)
            cache[i] = (seq_t)12;
        else
        {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];

            /* The multipliers 4, 6, 9 are small constants. */
            cache[i] = ( (seq_t)4 * t1 ) + ( (seq_t)6 * t2 ) - ( (seq_t)9 * t3 );
        }
    }

    cache_size = new_size;
    return cache[n];
}
