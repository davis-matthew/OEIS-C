/*
 * A021737  Decimal expansion of 1/733.
 * https://oeis.org/A021737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit after the decimal point of 1/733. It is
 *            computed by long division: remainder *= 10, digit = remainder /
 *            733, remainder %= 733
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1;
    const seq_t denom = 733;
    for (idx_t i = 0; i < n; ++i)
    {
        rem = rem * 10;
        rem = rem % denom;
    }
    seq_t digit = (rem * 10) / denom;
    return digit;
}
