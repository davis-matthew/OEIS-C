/*
 * A021973  Decimal expansion of 1/969.
 * https://oeis.org/A021973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (⟦10^n * 1/969⟧ mod 10). The sequence consists of the
 *            decimal digits of 1/969 after the decimal point (a(0) is the
 *            first digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already generated digits */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    /* Remainder after the last generated digit; starts with 1 (the numerator) */
    static uint64_t rem = 1;
    /* Allocate initial cache on first call */
    if (cache == NULL)
    {
        cap   = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }
    /* Extend cache and generate digits until we have at least n+1 terms */
    while (len <= n)
    {
        if (len == cap)
        {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }
        /* Multiply current remainder by 10, extract next digit, update remainder */
        uint64_t mul   = rem * 10;
        uint64_t digit = mul / 969;
        rem            = mul % 969;
        cache[len++]   = (seq_t)digit;
    }
    return cache[n];
}
