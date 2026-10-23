/*
 * A036663  Decimal expansion of 1/98019801.
 * https://oeis.org/A036663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1) / 98019801)) mod 10 (the n‑th digit after
 *            the decimal point of 1/98019801)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Denominator of the fraction 1/98019801 */
    static const idx_t D = 98019801u;
    idx_t r = 1;                     /* remainder before the n-th step   */
    for (idx_t i = 0; i < n; ++i) {  /* advance to the n-th digit      */
        r = (r * 10) % D;
    }
    /* Digit is floor(10 * remainder / D) */
    seq_t digit = (seq_t)((r * 10) / D);
    return digit;
}
