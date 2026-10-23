/*
 * A040384  Continued fraction for sqrt(405).
 * https://oeis.org/A040384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 20; for n > 0, a(n) = 8 if n is odd, 40 if n is even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;

    /* n is unsigned, so we can test the least‑significant bit */
    if ((n & 1) != 0)
        return (seq_t)8;

    return (seq_t)40;
}
