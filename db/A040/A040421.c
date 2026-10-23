/*
 * A040421  Continued fraction for sqrt(443).
 * https://oeis.org/A040421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040421: continued fraction terms for sqrt(443).
   The terms are a(0)=21, and thereafter alternating 21,42:
   a(n) = 21 if n==0 or n is odd; otherwise a(n) = 42. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)21;
    if ((n & (idx_t)1) == (idx_t)1)
        return (seq_t)21;
    return (seq_t)42;
}
