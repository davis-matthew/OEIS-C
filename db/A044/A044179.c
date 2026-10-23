/*
 * A044179  Numbers n such that string 5,6 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 49*n - 8
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 49 - 8;
}
