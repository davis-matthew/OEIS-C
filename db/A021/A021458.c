/*
 * A021458  Decimal expansion of 1/454.
 * https://oeis.org/A021458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of decimal expansion of 1/454 obtained by long
 *            division
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;
        digit = rem / (seq_t)454;
        rem %= (seq_t)454;
    }
    return digit;
}
