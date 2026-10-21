/*
 * A074850  Partial products of successive digits in the decimal expansion of
 *          Pi.
 * https://oeis.org/A074850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = product_{k=1}^{n} pi(k) where pi(k) is the k-th decimal
 *            digit of Pi (starting with 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 32 the product contains a zero digit (the 33rd decimal digit of Pi), so a(n) = 0. */

#include <stdint.h>

/* Digits of Pi starting from the integer part: 3.14159265358979323846264338327950... */
static const unsigned char pi_digits[34] = {
    3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4,
    6, 2, 6, 4, 3, 3, 8, 3, 2, 7, 9, 5, 0 /* 33rd digit, needed for the zero check */
};

seq_t a(idx_t n)
{
    /* For n < 1 defined as 0 (offset is 1). */
    if (n < 1)
        return 0;

    /* All terms beyond the 32nd digit are zero because digit 33 is 0. */
    if (n > 32)
        return 0;

    /* Compute the product of the first n digits. */
    seq_t prod = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        prod *= (seq_t)pi_digits[i];
    }
    return prod;
}
