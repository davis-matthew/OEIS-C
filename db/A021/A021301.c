/*
 * A021301  Decimal expansion of 1/297.
 * https://oeis.org/A021301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/297) mod 10 (period 6: 0,0,3,3,6,7)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {0, 0, 3, 3, 6, 7};
    return pat[n % 6];
}
