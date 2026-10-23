/*
 * A124156  Thickness of complete graph K_n.
 * https://oeis.org/A124156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+7)/6), except a(9)=a(10)=3
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 9 || n == 10)
        return (seq_t)3;
    return ((seq_t)n + (seq_t)7) / (seq_t)6;
}
