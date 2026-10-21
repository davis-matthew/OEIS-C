/*
 * A207539  Dodecanacci numbers (12th-order Fibonacci sequence): a(n) = a(n-1)
 *          +...+ a(n-12) with a(0)=...=a(11)=1.
 * https://oeis.org/A207539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+a(n-2)+…+a(n-12) with a(0)=…=a(11)=1
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */
/* Cache the values of the sequence so subsequent calls are fast. */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache      = NULL;   /* array of values 0 .. cache_cap-1 */
    static size_t cache_cap  = 0;      /* capacity of cache array          */
    static idx_t  last       = (idx_t)-1; /* largest n that has been fully computed */
    static bool    init       = false; /* whether initial 12 entries have been filled */

    /* The first twelve values are all 1. */
    if (n < 12)
        return (seq_t)1;

    /* Ensure the cache is large enough. */
    size_t needed = (size_t)n + 1;           /* we need indices 0..n inclusive */
    if (cache == NULL || needed > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)                      /* allocation failed */
            exit(1);
        cache = tmp;
        cache_cap = needed;

        /* First time the cache is enlarged we fill the first 12 values. */
        if (!init)
        {
            size_t lim = cache_cap < 12 ? cache_cap : 12;
            for (size_t i = 0; i < lim; ++i)
                cache[i] = (seq_t)1;
            init = true;
        }
    }

    /* Compute missing values up to n. */
    size_t start = (last == (idx_t)-1) ? 12 : (size_t)(last + 1);
    if (start < 12)                 /* never start before the first non‑initial term */
        start = 12;

    for (size_t k = start; k <= (size_t)n; ++k)
    {
        seq_t sum = (seq_t)0;
        /* add the previous twelve terms */
        for (size_t i = 1; i <= 12; ++i)
            sum += cache[k - i];
        cache[k] = sum;
    }
    last = (idx_t)n;                 /* record highest computed index */

    return cache[(size_t)n];
}
