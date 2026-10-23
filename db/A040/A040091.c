/*
 * A040091  Continued fraction for sqrt(102).
 * https://oeis.org/A040091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 10; for n > 0, a(n) = 10 if n is odd, otherwise 20
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is the first term of the continued fraction for sqrt(102) */
    if (n == 0)
        return (seq_t)10;
    /* Even indices after the first term correspond to 20, odd to 10 */
    return (seq_t)((n & 1U) ? 10 : 20);
}
