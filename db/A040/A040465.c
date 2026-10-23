/*
 * A040465  Continued fraction for sqrt(488).
 * https://oeis.org/A040465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 22; for n > 0, a(n) = 11 if n is odd, else 44 (periodic
 *            continued fraction of sqrt(488))
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;
    /* n >= 1 */
    return (n & 1) ? (seq_t)11 : (seq_t)44;
}
