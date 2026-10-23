/*
 * A044712  Numbers n such that string 8,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 81*n - 1
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)81 * nn - (seq_t)1;
}
