/*
 * A021541  Decimal expansion of 1/537.
 * https://oeis.org/A021541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 537) mod 10, i.e. the n‑th digit after
 *            the decimal point of 1/537
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t digit = 0;
    seq_t rem   = 1;                 /* remainder before generating a digit */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                   /* bring next decimal place */
        digit = rem / 537;           /* integer part of the quotient */
        rem %= 537;                  /* keep remainder for the next step */
    }
    return digit;
}
