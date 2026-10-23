/*
 * A040567  Continued fraction for sqrt(592).
 * https://oeis.org/A040567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24; for n>=1, a(n)=3 if n is odd, otherwise a(n)=48
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;
    return (n & (idx_t)1) ? (seq_t)3 : (seq_t)48;
}
