/*
 * A040343  Continued fraction for sqrt(363).
 * https://oeis.org/A040343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=19, a(1)=19; for n>=2, a(n)=38 if n even, else 19
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;
    /* for n>=1, odd indices give 19, even indices give 38 */
    return (n & (idx_t)1) ? (seq_t)19 : (seq_t)38;
}
