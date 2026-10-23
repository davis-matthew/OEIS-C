/*
 * A021599  Decimal expansion of 1/595.
 * https://oeis.org/A021599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of 1/595 (a(0) is the first digit
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 595 = 5 * 7 * 17, denominator of the fraction */
    static const idx_t denom = 595u;

    idx_t rem = 1;        /* current remainder */
    idx_t digit = 0;      /* current digit */

    /* Perform long division up to the n-th digit */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem = rem * 10;
        digit = rem / denom;
        rem  %= denom;
    }

    return (seq_t)digit;
}
