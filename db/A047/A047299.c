/*
 * A047299  Numbers that are congruent to {0, 1, 3, 4, 6} mod 7.
 * https://oeis.org/A047299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7n-5)/5)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n>=1; calculation in 64-bit idx_t ensures no overflow for the
       expected range of n.  The final value is cast to seq_t. */
    idx_t temp = 7 * n - 5;
    return (seq_t)(temp / 5);
}
