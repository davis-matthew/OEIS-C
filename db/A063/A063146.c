/*
 * A063146  Dimension of the space of weight 2n cusp forms for Gamma_0( 78 ).
 * https://oeis.org/A063146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*n - 18 for n > 1 a(1) = 11 This directly follows from
 *            the generating function x*(11+16*x+x^2)/(1-x)^2 and the
 *            recurrence a(n) = 2*a(n-1) - a(n-2) for n > 3
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 11;
    /* Convert to seq_t before arithmetic to avoid overflow in idx_t */
    return (seq_t)28 * (seq_t)n - (seq_t)18;
}
