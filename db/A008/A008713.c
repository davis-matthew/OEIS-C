/*
 * A008713  16 in base 16-n.
 * https://oeis.org/A008713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full, nice
 * Formula:   a(n) is the decimal integer formed by the base-(16-n)
 *            representation of the decimal number 16
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is only defined for 0 ≤ n ≤ 15.
       For n > 15 the base is non‑positive, so return 0. */
    if (n > 15)
        return 0;

    idx_t base = 16 - n;           /* 1 ≤ base ≤ 16 */
    seq_t result = 0;

    if (base == 1) {                 /* unary representation */
        /* The number 16 in base 1 is sixteen 1's. */
        for (idx_t i = 0; i < 16; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* For bases 2 to 16, compute the remainder digits. */
    seq_t temp = 16;
    seq_t rev_digits[20];           /* enough for any base in this range */
    idx_t count = 0;

    while (temp > 0) {
        rev_digits[count++] = temp % base;
        temp = temp / base;
    }

    /* Build the decimal number from most significant digit to least. */
    for (idx_t i = count; i > 0; --i)
        result = result * 10 + rev_digits[i - 1];

    return result;
}
