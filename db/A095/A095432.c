/*
 * A095432  a(n) = 17 written in base n.
 * https://oeis.org/A095432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by concatenating the base‑n
 *            digits of the decimal number 17 (each digit is written as
 *            decimal digits, e.g. the digit 14 is written as “14”)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Base 1: 17 is represented by seventeen 1’s */
    if (n == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 17; ++i)
            val = (seq_t)((seq_t)val * 10 + 1);
        return val;
    }

    /* For bases greater than 17, 17 is a single digit (value 17),
       which is written as the two decimal digits “17” → decimal 17. */
    if (n > 17) {
        return (seq_t)17;
    }

    /* Compute the base‑n representation of 17. */
    uint64_t temp = (uint64_t)17;      /* 17 fits easily into 64 bits */
    uint8_t digits[16];                /* maximum of 5 digits, array size generous */
    idx_t   len = 0;

    while (temp > 0) {
        digits[len++] = (uint8_t)(temp % (uint64_t)n);
        temp /= (uint64_t)n;
    }

    /* Convert the digit array to a decimal integer.
       Digits 0‑9 become one decimal digit, digits 10‑15 become two
       decimal digits (e.g. 12 → “12”). */
    seq_t val = 0;
    while (len > 0) {
        uint8_t d = digits[--len];
        if (d < 10) {
            val = (seq_t)((seq_t)val * 10 + d);
        } else {
            val = (seq_t)((seq_t)val * 100 + d);  /* two decimal digits */
        }
    }
    return val;
}
