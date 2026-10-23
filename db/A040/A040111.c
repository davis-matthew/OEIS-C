/*
 * A040111  Continued fraction for sqrt(123).
 * https://oeis.org/A040111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=11; for n≥1, a(n)=11 if n is odd, a(n)=22 if n is even
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    if (n & ((idx_t)1))
        return (seq_t)11;
    return (seq_t)22;
}
