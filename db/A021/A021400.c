/*
 * A021400  Decimal expansion of 1/396.
 * https://oeis.org/A021400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the decimal expansion of 1/396.
   a(0) = 0 (integer part).
   a(n) = the digit at position n+1 of the fractional part of 1/396
          (i.e. a(1)=first digit after the point,
           a(2)=second digit after the point, etc.)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* On first call, allocate a cache to store digits.
       digits[0] is unused, digits[1] holds the first fractional digit. */
    static seq_t *digits = NULL;    /* cache of digits */
    static size_t cached  = 0;      /* number of digits cached (max index) */
    static seq_t remainder = 1U;    /* current remainder after cached digits */
    const seq_t divisor = 396U;     /* denominator */

    if (n == 0U)                  /* integer part */
        return 0U;

    /* we need digit at fractional position pos = n+1, 1‑based */
    size_t pos = (size_t)(n) + 1U;

    /* extend cache if necessary */
    if (pos > cached) {
        /* grow the digits array to accommodate indices up to pos */
        size_t new_cap = pos + 1U;          /* include index 0 */
        seq_t *new_digits = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!new_digits)                  /* sanity check */
            return 0U; /* or abort */
        digits = new_digits;

        /* compute new digits sequentially */
        for (size_t i = cached + 1U; i <= pos; ++i) {
            remainder *= 10U;
            seq_t digit = remainder / divisor;
            remainder %= divisor;
            digits[i] = digit;
        }
        cached = pos;
    }

    return digits[pos];
}
