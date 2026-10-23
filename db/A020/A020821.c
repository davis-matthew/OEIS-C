/*
 * A020821  Decimal expansion of 1/8.
 * https://oeis.org/A020821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit of the decimal expansion of 1/8 (offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    else if (n == 1) return (seq_t)2;
    else if (n == 2) return (seq_t)5;
    else           return (seq_t)0;
}
