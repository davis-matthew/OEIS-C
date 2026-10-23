/*
 * A040376  Continued fraction for sqrt(396).
 * https://oeis.org/A040376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 19; for n > 0, a(n) = { 38 if n % 4 == 0, 8 if n % 4 ==
 *            2, 1 otherwise (n % 4 == 1 or 3) }
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;
    idx_t r = n & 3; /* n % 4 for unsigned idx_t */
    if (r == 0)
        return (seq_t)38;
    if (r == 2)
        return (seq_t)8;
    return (seq_t)1;
}
