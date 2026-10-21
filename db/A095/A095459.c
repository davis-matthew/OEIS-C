/*
 * A095459  a(n) = 33 written in base 16 - n.
 * https://oeis.org/A095459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digits of decimal 33 in base (16-n), interpreted as a
 *            decimal number
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 14, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 33 is the decimal number we need to express in base (16−n). */
    const unsigned int NUM = 33;
    /* Base decreases from 16 down to 1.  If n > 16 the base would be <= 0,
       which is not defined for this sequence – we return 0 in that case. */
    if (n > 16U) {
        return (seq_t)0;
    }

    unsigned int base = 16U - (unsigned int)n;  /* base is at least 1 */

    /* Special case: base 1 (unary).  The representation is NUM copies of
       the digit 1, so we build the decimal number consisting of NUM '1's. */
    if (base == 1U) {
        seq_t value = 0;
        for (unsigned int i = 0; i < NUM; ++i) {
            value = value * 10 + 1;
        }
        return value;
    }

    /* Compute the digits of NUM in the given base.  The digits are stored
       in reverse order for convenience. */
    unsigned int digits[32];   /* 33 decimal in base 2 fits in at most 6 digits, so 32 is safe */
    int dcnt = 0;
    unsigned int tmp = NUM;
    while (tmp > 0) {
        digits[dcnt++] = tmp % base;
        tmp /= base;
    }

    /* Build the decimal number formed by these digits. */
    seq_t value = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        value = value * 10 + digits[i];
    }
    return value;
}
