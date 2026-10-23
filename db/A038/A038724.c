/*
 * A038724  Each term describes the ordinal number of its position in the
 *          sequence, by a summation of the numerals in that ordinal number
 *          and then by the differences between each contiguous pair of the
 *          numerals in the order in which they appear.
 * https://oeis.org/A038724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038724: a(n) is obtained by concatenating the sum of the decimal
   digits of n (in decimal) with the absolute differences between each
   pair of consecutive digits of n, taken in order.  For n < 10 the result
   is simply n itself. */
#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t etc. */

seq_t a(idx_t n)
{
    /* n < 10 : only a single digit, no differences */
    if (n < 10)
        return (seq_t)n;

    /* Extract decimal digits of n into an array most‑significant first */
    /* The maximum number of decimal digits for a 64‑bit integer is 20 */
    uint8_t tmp[20];
    size_t len = 0;
    idx_t t = n;
    while (t > 0)
    {
        tmp[len++] = (uint8_t)(t % 10);
        t /= 10;
    }
    /* Reorder to most significant first */
    uint8_t digits[20];
    for (size_t i = 0; i < len; ++i)
        digits[i] = tmp[len - 1 - i];

    /* Sum of digits */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (seq_t)digits[i];

    /* Build the result by attaching the differences */
    seq_t result = sum;
    for (size_t i = 0; i + 1 < len; ++i)
    {
        uint8_t d1 = digits[i];
        uint8_t d2 = digits[i + 1];
        uint8_t diff = d1 > d2 ? d1 - d2 : d2 - d1;   /* absolute difference */
        result = result * (seq_t)10 + (seq_t)diff;
    }

    return result;
}
