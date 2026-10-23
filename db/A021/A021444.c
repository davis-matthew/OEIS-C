/*
 * A021444  Decimal expansion of 1/440.
 * https://oeis.org/A021444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit after the decimal point in the expansion
 *            of 1/440
 * Range:     u8 n <= 1, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: simulate long division: repeatedly multiply the current remainder
   by 10, divide by 440 to obtain the digit, then take the modulus to update the remainder. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* 440 fits comfortably in a small integer; the remainder never exceeds 439. */
    seq_t rem = 1;  /* starting remainder is the numerator of 1/440 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * (seq_t)10;       /* shift the decimal point to the next digit */
        digit = rem / (seq_t)440;    /* integer division gives the current digit */
        rem   = rem % (seq_t)440;    /* update remainder for the next step */
    }

    return digit;   /* digit is 0-9 and fits in seq_t */
}
