/*
 * A020806  Decimal expansion of 1/7.
 * https://oeis.org/A020806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = {1,4,2,8,5,7}[n mod 6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = { (seq_t)1, (seq_t)4, (seq_t)2, (seq_t)8, (seq_t)5, (seq_t)7 };
    return pat[n % 6];
}
