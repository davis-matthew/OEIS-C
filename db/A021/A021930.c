/*
 * A021930  Decimal expansion of 1/926.
 * https://oeis.org/A021930
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

/* Sequence: decimal digits of 1/926 (after the decimal point) */
/* a(n) = the n-th digit (0-indexed) of the decimal expansion of 1/926. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache of already computed digits */
    static seq_t *cache = NULL;      /* array of digits */
    static size_t cache_cap = 0;     /* allocated capacity  */
    static idx_t next_index = 0;     /* next index to compute (first not-yet-filled) */
    static uint64_t remainder = 1;   /* remainder of 1/926 after generating next_index-1 digits */

    /* If the requested index is not yet computed, generate digits up to n */
    if (next_index <= n)
    {
        /* Ensure the cache has enough space */
        size_t needed = (size_t)(n + 1);               /* we need indices 0..n inclusive */
        if (needed > cache_cap)
        {
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (tmp == NULL)                          /* allocation failure – abort */
                abort();
            cache = tmp;
            cache_cap = needed;
        }

        /* Generate the missing digits via long division */
        for (idx_t i = next_index; i <= n; ++i)
        {
            remainder *= 10ULL;                       /* shift remainder left by one decimal position */
            seq_t digit = (seq_t)(remainder / 926ULL);/* the next decimal digit */
            remainder %= 926ULL;                      /* new remainder */
            cache[i] = digit;
        }

        next_index = n + 1;                           /* all indices up to n are now computed */
    }

    return cache[n];
}
