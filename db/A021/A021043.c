/*
 * A021043  Decimal expansion of 1/39.
 * https://oeis.org/A021043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digits of 1/39 (decimal) which cycle 0,2,5,6,4,1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period six: 0,2,5,6,4,1 */
    static const seq_t pat[6] = {0, 2, 5, 6, 4, 1};
    return pat[n % 6];
}
