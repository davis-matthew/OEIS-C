/*
 * A040463  Continued fraction for sqrt(486).
 * https://oeis.org/A040463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=22; for n>0, a(n)=22 if n is odd, otherwise a(n)=44
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; we can test n==0 or parity */
    if (n == 0)
        return 22;
    /* use bitwise AND for odd check – works for any integer width */
    if (n & 1)
        return 22;
    return 44;
}
