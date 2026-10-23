/*
 * A021564  Decimal expansion of 1/560.
 * https://oeis.org/A021564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/560
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 7;
    static const seq_t period[6] = {8, 5, 7, 1, 4, 2};
    return period[(n - 4) % 6];
}
