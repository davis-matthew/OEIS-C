/*
 * A040869  Continued fraction for sqrt(899).
 * https://oeis.org/A040869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, mult, easy
 * Formula:   a(0) = 29, and for n ≥ 1 * a(n) = 58 if n is even * 1 if n is
 *            odd
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;
    /* n is unsigned, so n & 1 is 0 for even, 1 for odd */
    return (n & 1) ? (seq_t)1 : (seq_t)58;
}
