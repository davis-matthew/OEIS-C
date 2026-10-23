/*
 * A033645  Base 8 digital convolution sequence.
 * https://oeis.org/A033645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ (digit_i in base 8 of n) * a(n-1-i)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1                                      */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed a[k]  */
    static idx_t   cache_len = 0;    /* number of elements stored  */
    /* allocate initial cache for a(0) */
    if (cache_len == 0)
    {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0; /* fallback on allocation failure */
        cache[0] = (seq_t)1;         /* a(0) = 1 */
    }
    /* if already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (cache == NULL) return 0; /* allocation failure */

    /* compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t result = 0;
        idx_t temp   = i;        /* integer to extract digits */
        idx_t index  = i - 1;    /* corresponding a(index) values */

        while (temp > 0)
        {
            uint64_t digit = temp % 8;   /* base‑8 digit */
            temp          = temp / 8;
            if (digit != 0)
                result += (seq_t)digit * cache[index];
            /* move to the next more significant digit */
            --index;
        }
        cache[i] = result;
    }
    cache_len = n + 1;
    return cache[n];
}
