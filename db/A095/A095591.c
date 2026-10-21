/*
 * A095591  a(n) = 99 written in base 16 - n.
 * https://oeis.org/A095591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = write the number 99 in base (16 - n) * and interpret the
 *            resulting digit string as a * decimal number. For n = 15 the
 *            base is 1, * so 99 is represented by 99 ones (unary)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for 0 ≤ n ≤ 15.
     * For other indices we return 0 (undefined). */
    if (n > 15u)
        return (seq_t)0;

    idx_t base = 16u - n;

    /* Unary base (n = 15).  99 in base 1 is 99 ones. */
    if (base == 1u) {
        seq_t result = 0;
        for (idx_t i = 0; i < 99; ++i)
            result = result * (seq_t)10 + (seq_t)1;
        return result;
    }

    /* For bases 2 … 15 we compute the representation of 99
     * in that base and then treat the digit string as a decimal
     * number.  The decimal digits are always ≤ 9 in this range. */

    seq_t value = 99;                /* 99 in the chosen base */
    uint8_t digits[8];               /* maximum 8 digits for base 2 */
    idx_t len = 0;

    while (value > 0) {
        digits[len++] = (uint8_t)(value % base);
        value = value / base;
    }

    seq_t result = 0;
    for (idx_t i = len; i > 0; ) {
        --i;
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    return result;
}
