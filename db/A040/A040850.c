/*
 * A040850  Continued fraction for sqrt(880).
 * https://oeis.org/A040850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 29; for n > 0, a(n) = 58 if 4 divides n, otherwise a(n) =
 *            1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;

    /* n > 0 : return 58 for multiples of 4, 1 otherwise */
    return ((n & 3) == 0) ? (seq_t)58 : (seq_t)1;
}
