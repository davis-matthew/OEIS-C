/*
 * A063157  Dimension of the space of weight 2n cusp forms for Gamma_0( 89 ).
 * https://oeis.org/A063157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n > 1, a(n) = ((-1)^n + 30*n - 17) / 2. This
 *            simplifies to: a(n) = 15*n - 8 if n is even, a(n) = 15*n - 9 if
 *            n is odd
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;

    seq_t nn = (seq_t)n;
    if ((n & 1) == 0)           /* even n */
        return (seq_t)15 * nn - (seq_t)8;
    else                         /* odd n */
        return (seq_t)15 * nn - (seq_t)9;
}
