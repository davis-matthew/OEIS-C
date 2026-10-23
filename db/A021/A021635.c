/*
 * A021635  Decimal expansion of 1/631.
 * https://oeis.org/A021635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Digit of decimal expansion of 1/631.
 * The sequence repeats with period ≤ 630 (the multiplicative order of 10 modulo 631).
 * a(0) is the first digit after the decimal point.
 */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* static buffer for the repeating cycle */
    static seq_t digits[630];
    static size_t period = 0;

    /* initialise once – compute the repeating digits of 1/631 */
    if (period == 0) {
        seq_t r = 1;          /* remainder after each digit */
        size_t pos = 0;
        do {
            seq_t d = (r * 10) / 631;     /* next digit */
            digits[pos++] = d;
            r = (r * 10) % 631;           /* update remainder */
        } while (r != 1);                 /* end of period */
        period = pos;
    }

    /* return the digit at position n (modulo the period) */
    return digits[(size_t)(n % period)];
}
