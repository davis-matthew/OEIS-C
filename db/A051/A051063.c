/*
 * A051063  27*n+9 or 27*n+18.
 * https://oeis.org/A051063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*floor((n-1)/2) + (9 if n is odd, otherwise 18)
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n - 1) / 2;           /* floor((n-1)/2) */
    seq_t val = ((seq_t)27) * m;           /* 27 * m */
    val += (n & 1) ? 9 : 18;               /* add 9 for odd n, 18 for even n */
    return val;
}
