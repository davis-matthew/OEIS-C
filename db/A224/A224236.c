/*
 * A224236  Decimal expansion of speed of light in feet per second.
 * https://oeis.org/A224236
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224236: decimal digits of 299792458/0.3048
   (decimal expansion of the speed of light in feet per second) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 9) {
        /* below the offset, behaviour does not matter */
        return (seq_t)0;
    }

    /* static cache of computed digits, starting from the first digit of
       the integer part of the number 299792458/0.3048 */
    static seq_t      *digits = NULL;
    static size_t     digits_capacity = 0;
    static size_t     digits_count    = 0;
    static uint64_t   rem              = 0;   /* current remainder for decimal part */

    /* constants of the rational number */
    const uint64_t numer = 374740572500ULL;   /* 299792458 * 1250 */
    const uint64_t denom = 381ULL;            /* 1250 / 381 = 1 / 0.3048 */

    /* initialize the cache on the first call */
    if (!digits) {
        /* integer part of the value */
        uint64_t integer_part = numer / denom;
        rem = numer % denom;       /* remainder for the fractional part */

        /* compute number of digits in the integer part */
        uint64_t t = integer_part;
        size_t lead_digits = 0;
        do {
            t /= 10;
            lead_digits++;
        } while (t);

        /* allocate storage: integer digits + some room for decimals */
        digits_capacity = lead_digits + 16;
        digits = (seq_t *)malloc(digits_capacity * sizeof(seq_t));

        /* store integer part digits from most significant to least */
        t = integer_part;
        size_t pos = lead_digits;
        do {
            uint8_t d = t % 10;
            digits[--pos] = (seq_t)d;
            t /= 10;
        } while (t);

        digits_count = lead_digits;
    }

    /* position of the requested digit (1‑based) */
    size_t pos = (size_t)(n - 8);

    /* extend the cache until the requested position is available */
    while (pos > digits_count) {
        if (digits_count >= digits_capacity) {
            digits_capacity *= 2;
            digits = (seq_t *)realloc(digits,
                                      digits_capacity * sizeof(seq_t));
        }

        /* long division step to obtain next decimal digit */
        rem *= 10;
        uint64_t d = rem / denom;
        rem   %= denom;
        digits[digits_count++] = (seq_t)d;
    }

    return digits[pos - 1];
}
