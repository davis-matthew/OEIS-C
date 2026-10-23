/*
 * A021028  Decimal expansion of 1/24.
 * https://oeis.org/A021028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0, a(1) = 4, a(2) = 1, a(n) = 6 for n >= 3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 1;
    return 6;
}
