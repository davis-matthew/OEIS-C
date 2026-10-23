/*
 * A021721  Decimal expansion of 1/717.
 * https://oeis.org/A021721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) repeats with period 7: a(0)=0, a(1)=0, a(2)=1, a(3)=3,
 *            a(4)=9, a(5)=4, a(6)=7
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digits of the decimal expansion of 1/717 repeat with period 7 */
    static const seq_t period[7] = {0, 0, 1, 3, 9, 4, 7};
    return period[n % 7];
}
