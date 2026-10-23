/*
 * A040074  Continued fraction for sqrt(84).
 * https://oeis.org/A040074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9978, u16 n <= 9978, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction for sqrt(84).
   a(0) = 9;
   for n >= 1: a(n) = 6 if n is odd, 18 if n is even. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;
    return ((n & (idx_t)1) ? (seq_t)6 : (seq_t)18);
}
