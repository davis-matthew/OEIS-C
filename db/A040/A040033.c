/*
 * A040033  Continued fraction for sqrt(40).
 * https://oeis.org/A040033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 8991, u16 n <= 8991, u32 n <= 8991, u64 n <= 8991,
 *            u128 n <= 8991, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040033: Continued fraction for sqrt(40)
   a(0) = 6, a(n) = 3 if n is odd, a(n) = 12 if n is even and n>0  */

/* Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)6;
    return (n & 1) ? (seq_t)3 : (seq_t)12;
}
