/*
 * A021180  Decimal expansion of 1/176.
 * https://oeis.org/A021180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/176) mod 10, i.e. the n‑th digit after the
 *            decimal point of 1/176
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 1;          /* current remainder of the long division */
    seq_t digit = 0;      /* the digit that will be returned */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;           /* shift remainder left by a decimal place */
        digit = (seq_t)(r / 176);  /* integer division gives the next digit */
        r %= 176;          /* keep only the remainder for the next step */
    }

    return digit;
}
