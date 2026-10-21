/*
 * A095511  a(n) = 59 written in base 11 - n.
 * https://oeis.org/A095511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal interpretation of 59 written in base (11-n)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  For n > 11 the base 11-n is ≤ 0, not a valid numeral system.  */
    if (n > 11) return (seq_t)0;

    int base = 11 - (int)n;   /* n ≤ 11, so base ≥ 1 */

    /* The 59–representation in base 1 is 59 consecutive 1's. */
    if (base == 1) {
        seq_t result = 0;
        for (int i = 0; i < 59; ++i)
            result = result * 10 + 1;   /* append digit 1 */
        return result;
    }

    /* For bases ≥ 2 obtain the digits of 59 in that base. */
    int digits[64];
    int dcnt = 0;
    int value = 59;
    while (value > 0) {
        digits[dcnt++] = value % base;
        value /= base;
    }

    /* Read the digits as a decimal number. */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
