/*
 * A021222  Decimal expansion of 1/218.
 * https://oeis.org/A021222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021222: digit n of the decimal expansion of 1/218
   (a(0) = first digit after the decimal point, a(1) = second digit, etc.) */
#include <stdint.h>

seq_t a(idx_t n) {
    /* 1/218 = 0.0...; compute each decimal digit by long division.
       The remainder never exceeds 217, so 64‑bit arithmetic is ample. */
    uint64_t r = 1;        /* current remainder before multiplying by 10 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                 /* shift one decimal place */
        digit = (seq_t)(r / 218);/* digit is the integer part of the division */
        r %= 218;                /* remainder for next step */
    }
    return digit;
}
