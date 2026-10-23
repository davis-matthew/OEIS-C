/*
 * A040094  Continued fraction for sqrt(105).
 * https://oeis.org/A040094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=10; for n>0, a(n)=4 if n is odd, a(n)=20 if n is even. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)10;
    return ((n & 1) ? (seq_t)4 : (seq_t)20);
}
