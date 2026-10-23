/*
 * A142464  Decimal expansion of 13/36.
 * https://oeis.org/A142464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=3, a(1)=6, a(n)=1 for n >= 2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    else if (n == 1)
        return (seq_t)6;
    else
        return (seq_t)1;
}
