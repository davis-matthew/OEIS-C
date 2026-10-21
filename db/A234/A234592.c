/*
 * A234592  Number of binary words of length n which have no 0^b 1 1 0^a 1 0 1
 *          0^b - matches, where a=b=2.
 * https://oeis.org/A234592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-9) + a(n-10) + a(n-11) for n ≥ 11
   a(n) = 2^n for 0 ≤ n ≤ 10                           */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_max = 10;   /* highest index computed */

    /* first call: allocate and fill base values 0..10 */
    if (!cache)
    {
        cache_cap = (n > 10 ? n + 1 : 11);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = ((seq_t)1) << i;   /* 2^i */
        computed_max = 10;
    }

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = (n + 1) * 2;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    while (computed_max < n)
    {
        idx_t i = computed_max + 1;
        if (i <= 10)
            cache[i] = ((seq_t)1) << i;   /* 2^i */
        else
            cache[i] = cache[i - 1] * ((seq_t)2)
                     - cache[i - 9]
                     + cache[i - 10]
                     + cache[i - 11];
        ++computed_max;
    }

    return cache[n];
}
