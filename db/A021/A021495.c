/*
 * A021495  Decimal expansion of 1/491.
 * https://oeis.org/A021495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n in the decimal expansion of 1/491
 *            (0-indexed) computed by performing long division
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Remainder after extracting previous digits */
    seq_t rem = 1;                 /* start with numerator 1 */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                 /* shift one decimal place */
        seq_t digit = rem / 491;   /* next digit */
        rem     %= 491;            /* remainder for next step */
        if (i == n)
            return digit;
    }
    /* This point is never reached, but keep the compiler happy */
    return 0;
}
