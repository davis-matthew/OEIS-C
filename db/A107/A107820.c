/*
 * A107820  a(1)=3, a(2)=5; thereafter a(n) = n+5.
 * https://oeis.org/A107820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=3; a(2)=5; a(n)=n+5 for n>=3
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    return (seq_t)n + (seq_t)5;
}
