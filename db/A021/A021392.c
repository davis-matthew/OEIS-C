/*
 * A021392  Decimal expansion of 1/388.
 * https://oeis.org/A021392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of 1/388 at index n in its decimal expansion
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int remainder = 1;
    unsigned int digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        remainder *= 10;
        digit = remainder / 388;   /* 0 … 9 */
        remainder %= 388;
    }
    return (seq_t)digit;
}
