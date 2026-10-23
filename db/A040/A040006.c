/*
 * A040006  Continued fraction for sqrt(10).
 * https://oeis.org/A040006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=3; a(n)=6 for n>0
 * Range:     u8 n <= 9495, u16 n <= 9495, u32 n <= 9495, u64 n <= 9495,
 *            u128 n <= 9495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)3 : (seq_t)6;
}
