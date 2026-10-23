/*
 * A040426  Continued fraction for sqrt(448).
 * https://oeis.org/A040426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 21; for n>0: a(n) = 6 if n is odd, 42 if n is even
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
    return (n & 1ULL) ? (seq_t)6 : (seq_t)42;
}
