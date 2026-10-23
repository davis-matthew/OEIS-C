/*
 * A040307  Continued fraction for sqrt(326).
 * https://oeis.org/A040307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 18 if n==0 or n odd; a(n) = 36 otherwise
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;
    return ((n & (idx_t)1) ? (seq_t)18 : (seq_t)36);
}
