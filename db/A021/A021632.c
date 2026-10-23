/*
 * A021632  Decimal expansion of 1/628.
 * https://oeis.org/A021632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/628, offset 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t denom = 628;
    idx_t rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;
        digit = (seq_t)(rem / denom);
        rem %= denom;
    }
    return digit;
}
