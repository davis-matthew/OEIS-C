/*
 * A040317  Continued fraction for sqrt(336).
 * https://oeis.org/A040317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=18, a(1)=3, a(n)=a(n-2) for n>=2. The sequence is: 18, 3,
 *            36, 3, 36, 3,
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)3;
    /* For n >= 2 : even indices give 36, odd indices give 3 */
    return ((n & (idx_t)1) == 0) ? (seq_t)36 : (seq_t)3;
}
