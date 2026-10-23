/*
 * A040316  Continued fraction for sqrt(335).
 * https://oeis.org/A040316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 18, for n>=1: a(n) = 36 if (n-1)%4==3 else 3
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)18;
    /* (n-1)%4 can be computed by bitwise AND with 3 */
    idx_t rem = (n - 1) & (idx_t)3;
    return (rem == 3) ? (seq_t)36 : (seq_t)3;
}
