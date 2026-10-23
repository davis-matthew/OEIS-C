/*
 * A040071  Continued fraction for sqrt(80).
 * https://oeis.org/A040071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 8; for n > 0, a(n) = 4^(1+(-1)^n)
 * Range:     u8 n <= 9967, u16 n <= 9967, u32 n <= 9967, u64 n <= 9967,
 *            u128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          which yields a(n) = 16 when n is even, 1 when n is odd. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    return ((n & 1) == 0) ? (seq_t)16 : (seq_t)1;
}
