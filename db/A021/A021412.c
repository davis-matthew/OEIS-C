/*
 * A021412  Decimal expansion of 1/408.
 * https://oeis.org/A021412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth digit after the decimal point in the decimal
 *            expansion of 1/408
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 1;          /* current remainder, always < 408 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10u;
        digit = (seq_t)(r / 408u);
        r %= 408u;
    }
    return digit;
}
