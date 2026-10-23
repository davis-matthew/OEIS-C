/*
 * A118087  Start with 1 and repeatedly reverse the digits and add 43 to get
 *          the next term.
 * https://oeis.org/A118087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 43 + reverse_decimal(a(n-1)), a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Reverse the decimal digits of a seq_t value */
static seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0)
    {
        rev = rev * 10 + x % 10;
        x = x / 10;
    }
    return rev;
}

/* Primary sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache     = NULL;
    static idx_t cache_size = 0;

    /* Initialise or grow the cache as needed */
    if (cache == NULL)
    {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return 0;    /* allocation failed */
        cache[0] = 1;     /* a(1) */
    }
    else if (cache_size < n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)
            return 0;    /* allocation failed */
        cache = tmp;
        /* initialise new entries to 0 so we know they are uncomputed */
        for (idx_t i = cache_size; i < n; ++i)
            cache[i] = 0;
        cache_size = n;
    }

    /* Compute any missing terms up to a(n) */
    for (idx_t i = 1; i < n; ++i)
    {
        if (cache[i] == 0)
        {
            cache[i] = 43 + reverse_decimal(cache[i - 1]);
        }
    }

    return cache[n - 1];
}
