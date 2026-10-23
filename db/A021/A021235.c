/*
 * A021235  Decimal expansion of 1/231.
 * https://oeis.org/A021235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of 1/231 at position n; equivalently a(n) =
 *            {0,0,4,3,2,9}[n mod 6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {0, 0, 4, 3, 2, 9};
    return pat[n % 6];
}
