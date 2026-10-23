/*
 * A040224  Continued fraction for sqrt(240).
 * https://oeis.org/A040224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=15; for n>=1, a(n) = (n%2?2:30)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)15;
    return (seq_t)((n & 1) ? 2 : 30);
}
