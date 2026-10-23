/*
 * A040563  Continued fraction for sqrt(588).
 * https://oeis.org/A040563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = a(n-2); a(0)=24, a(1)=4
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
    if (n & 1)   /* odd index */
        return (seq_t)4;
    /* even index >= 2 */
    return (seq_t)48;
}
