/*
 * A021575  Decimal expansion of 1/571.
 * https://oeis.org/A021575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/571) % 10 (decimal digit of 1/571)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local static state for caching computed digits */
    static seq_t *digits   = NULL;
    static idx_t size     = 0;   /* Number of digits currently stored */
    static idx_t capacity = 0;   /* Allocated capacity of digits array */
    static seq_t  rem      = 1;  /* Remainder after last computed decimal digit */

    const idx_t P = 571; /* Denominator of the fraction */

    /* If the requested digit is already cached, return it immediately */
    if (n < size)
        return digits[n];

    /* Expand the cache if necessary */
    idx_t need = n + 1;
    if (need > capacity) {
        idx_t newcap = capacity ? capacity * 2 : 1;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(digits, newcap * sizeof(seq_t));
        if (!tmp && newcap > 0) /* memory allocation failed – abort silently */
            return 0;
        digits   = tmp;
        capacity = newcap;
    }

    /* Compute missing digits starting from the current size */
    for (idx_t i = size; i < need; ++i) {
        rem   = rem * 10;        /* Shift remainder left by one decimal place */
        seq_t d = rem / P;       /* Next digit */
        rem   %= P;              /* Update remainder for the next iteration */
        digits[i] = d;
    }

    size = need;                  /* Update cached size */
    return digits[n];
}
