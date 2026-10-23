/*
 * A021398  Decimal expansion of 1/394.
 * https://oeis.org/A021398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point of 1/394 (a(0) is
 *            the first digit after the decimal point). Computed by long
 *            division: repeatedly multiply the remainder by 10, take the
 *            quotient as the next digit, and keep the modulus
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache of already computed digits */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored digits */
    static size_t cache_cap = 0;   /* allocated capacity */
    static uint32_t rem = 1;       /* current remainder (always < 394) */

    /* Grow the cache if necessary */
    if (n >= cache_len) {
        size_t required = n + 1;
        if (cache_cap < required) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 256;
            if (new_cap < required) new_cap = required;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate missing digits using long division */
        while (cache_len <= n) {
            uint32_t prod = rem * 10u;
            seq_t digit = (seq_t)(prod / 394u);
            rem = prod % 394u;
            cache[cache_len++] = digit;
        }
    }

    return cache[n];
}
