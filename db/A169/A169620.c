/*
 * A169620  Hankel transform of quintuple factorial numbers A008548.
 * https://oeis.org/A169620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} ((5*k+1)*(5*k+5))^(n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size of cache */
    static idx_t computed = 0;     /* index of last computed value (>= 0) */
    static seq_t pref = 1;         /* product of base_k for k=0..(computed-1) */

    /* initialise cache for the first call */
    if (!cache)
    {
        cache_size = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        computed = 0;
        pref = (seq_t)1;
    }

    /* enlarge cache if necessary */
    if (n >= cache_size)
    {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = newsize;
    }

    /* compute missing values up to n */
    if (computed < n)
    {
        for (idx_t i = computed + 1; i <= n; ++i)
        {
            /* base = (5*(i-1)+1) * (5*(i-1)+5) */
            seq_t base = (seq_t)(5 * (i - 1) + 1);
            base *= (seq_t)(5 * (i - 1) + 5);

            pref *= base;           /* update product of bases up to i-1 */
            cache[i] = cache[i - 1] * pref; /* a[i] = a[i-1] * pref */
        }
        computed = n;
    }

    return cache[n];
}
