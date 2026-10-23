/*
 * A040624  Continued fraction for sqrt(650).
 * https://oeis.org/A040624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 25; for n > 0, a(n) = 2 if n is odd, otherwise 50
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;
    return (seq_t)((n & 1) ? 2 : 50);
}
