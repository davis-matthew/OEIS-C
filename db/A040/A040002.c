/*
 * A040002  Continued fraction for sqrt(5).
 * https://oeis.org/A040002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, cons
 * Formula:   a(0)=2, a(n)=4 for n>0
 * Range:     u8 n <= 9480, u16 n <= 9480, u32 n <= 9480, u64 n <= 9480,
 *            u128 n <= 9480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    return (seq_t)4;
}
