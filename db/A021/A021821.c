/*
 * A021821  Decimal expansion of 1/817.
 * https://oeis.org/A021821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prefix: digits of the decimal expansion of 1/817.
   a(0) is the first digit after the decimal point. */
#include <stdlib.h>

/* Memoized computation of the nth digit of 1/817 in base 10. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* cached digits */
    static size_t cache_len = 0;          /* number of stored digits */
    static size_t cache_cap = 0;          /* allocated capacity */
    static unsigned int rem = 1;          /* current remainder of the long division */

    /* Grow cache to at least n+1 elements. */
    while (cache_len <= n) {
        if (cache_cap == 0) {
            cache_cap = 64;                                       /* initial size */
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cap <= cache_len) {
            cache_cap <<= 1;                                      /* double size */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Perform one step of the long division 1/817. */
        rem *= 10u;                                               /* shift left by one decimal place */
        unsigned int digit = rem / 817u;                           /* integer part */
        rem %= 817u;                                               /* new remainder */
        cache[cache_len++] = (seq_t)digit;                        /* store digit */
    }

    return cache[n];
}
