/*
 * A021890  Decimal expansion of 1/886.
 * https://oeis.org/A021890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit after the decimal point in the expansion
 *            of 1/886. It is computed by long division: repeatedly multiply
 *            the remainder by 10, take the integer part as the digit, and
 *            keep the new remainder
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t denom = 886ULL;
    idx_t rem = 1;          /* initial remainder = numerator of fraction 1/denom */
    idx_t i;
    seq_t digit = 0;        /* digit from current iteration */

    for (i = 0; i <= n; ++i) {
        rem *= 10;                     /* bring next decimal digit in */
        digit = rem / denom;           /* integer part = next digit */
        rem  %= denom;                 /* remainder for next step */
    }
    return digit;
}
