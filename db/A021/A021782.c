/*
 * A021782  Decimal expansion of 1/778.
 * https://oeis.org/A021782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of the decimal expansion of 1/778
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) is the first digit after the decimal point. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed digits.  Digits are stored in unsigned
       char to keep the cache compact for all build sizes.  The cache never
       shrinks; a single remainder value is maintained for the current
       fraction remainder during long division.  */
    static unsigned char *digits = NULL;   /* cached digits */
    static size_t digits_len   = 0;        /* number of digits cached */
    static unsigned long rem    = 1;       /* current remainder (0..777) */

    /* We need up to digit n inclusive.  Convert idx_t to size_t for
       indexing; the sequence is defined for many thousands of digits
       and SIZE_MAX is plenty large for all test cases. */
    const size_t target = (size_t)n;

    if (target + 1 > digits_len) {
        /* Ensure cache has enough capacity.  Double until big enough. */
        size_t new_cap = digits_len ? digits_len : 16;
        while (new_cap < target + 1)
            new_cap <<= 1;

        unsigned char *tmp = (unsigned char*)realloc(digits, new_cap * sizeof(unsigned char));
        /* If realloc fails, keep the old buffer; the partial cache is still
           usable for previously computed digits. */
        if (tmp)
            digits = tmp;

        /* Generate missing digits using long division: */
        for (size_t i = digits_len; i <= target; ++i) {
            unsigned long digit = (rem * 10) / 778;
            unsigned long next_rem = (rem * 10) % 778;
            digits[i] = (unsigned char)digit;
            rem = next_rem;
        }
        digits_len = target + 1;
    }

    return (seq_t)digits[target];
}
