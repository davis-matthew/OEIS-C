/*
 * A020989  a(n) = (5*4^n - 2)/3.
 * https://oeis.org/A020989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*4^n - 2)/3; recurrence a(0) = 1; a(n) = 4*a(n-1) + 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1; /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * 4 + 2;
    }
    return result;
}
