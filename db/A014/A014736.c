/*
 * A014736  Squares of odd triangular numbers.
 * https://oeis.org/A014736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( ( (n is even)?(2n+1):(2n) ) * ( (n is
 *            even)?(2n+2):(2n+1) ) / 2 )^2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 180, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the index of the odd triangular number */
    seq_t m = ((seq_t)n << 1) + ((n & 1) == 0 ? 1 : 0);

    /* compute the triangular number t = m*(m+1)/2 */
    seq_t t = m * (m + 1) / 2;

    /* return its square */
    return t * t;
}
