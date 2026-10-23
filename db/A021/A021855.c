/*
 * A021855  Decimal expansion of 1/851.
 * https://oeis.org/A021855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit n of the expansion of 1/851 after the decimal point. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  Cache for already-computed digits  */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* current allocation size */
    static idx_t   cached = 0;   /* number of digits already computed */
    static uint64_t rem    = 1;  /* remainder after last computed digit */

    /*  Return cached value if already computed  */
    if (n < cached)
        return cache[n];

    /*  Initialise cache on first use  */
    if (cache == NULL)
    {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /*  Compute successive digits up to index n  */
    for (idx_t i = cached; i <= n; ++i)
    {
        /*  Grow cache if necessary  */
        if (cached >= cap)
        {
            size_t new_cap = cap ? cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)   /*  allocation failed – return 0 as safe value  */
                return 0;
            cache = tmp;
            cap   = new_cap;
        }

        rem *= 10;
        seq_t digit = (seq_t)(rem / 851);
        rem %= 851;
        cache[cached++] = digit;
    }

    return cache[n];
}
