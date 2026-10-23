/*
 * A032542  Integer part of decimal 'base-5 looking' numbers divided by their
 *          actual base-5 values.
 * https://oeis.org/A032542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(A007091(n)/n)
 * Range:     u8 n <= 74, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007091(n) is the nth positive integer whose decimal digits are only 0–4.
   This is obtained by writing n in base‑5 and interpreting that string
   as a decimal number (i.e. treating the base‑5 digits as decimal digits). */

#include <stddef.h>   /* for size_t */

/* Compute A007091(n) and divide by n. */
seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is defined for n ≥ 1; 0 is not used */
        return 0;

    /* Extract base‑5 digits of n, least significant first. */
    idx_t temp = n;
    unsigned char digits[64]; /* enough for the range of n tested */
    size_t len = 0;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 5);
        temp /= 5;
    }

    /* Build the decimal number that has the same digit string. */
    seq_t base5_decimal = 0;
    for (size_t i = len; i > 0; --i) {
        base5_decimal = base5_decimal * 10 + digits[i - 1];
    }

    /* Return the integer part of the division. */
    return base5_decimal / (seq_t)n;
}
