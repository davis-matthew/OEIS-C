/*
 * A040433  Continued fraction for sqrt(455).
 * https://oeis.org/A040433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 21 a(n) = 3 if n is odd (n > 0) a(n) = 42 if n is even (n
 *            > 0) (Continued fraction terms of √455)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)21;
    else
        return (n & 1) ? (seq_t)3 : (seq_t)42;
}
