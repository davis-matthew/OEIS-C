/*
 * A040858  Continued fraction for sqrt(888).
 * https://oeis.org/A040858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   * a(0) = 29 * a(n) = 1 if n is odd * a(n) = 58 if n is even and
 *            (n/2) is even * a(n) = 3 if n is even and (n/2) is odd
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 29;
    if (n & 1)            /* n is odd */
        return 1;
    /* n is even */
    if (((n >> 1) & 1) == 0)   /* (n/2) is even */
        return 58;
    else
        return 3;
}
