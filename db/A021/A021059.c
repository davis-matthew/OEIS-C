/*
 * A021059  Decimal expansion of 1/55.
 * https://oeis.org/A021059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, mult, easy
 * Formula:   a(0)=0; for n>0, a(n)=8 if n is even, else 1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return ((n & (idx_t)1) == 0) ? (seq_t)8 : (seq_t)1;
}
