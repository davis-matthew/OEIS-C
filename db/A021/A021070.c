/*
 * A021070  Decimal expansion of 1/66.
 * https://oeis.org/A021070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, mult, easy
 * Formula:   a(0) = 0; for n >= 1, a(n) = 1 if n is odd, 5 if n is even
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return (n & 1) ? 1 : 5;
}
