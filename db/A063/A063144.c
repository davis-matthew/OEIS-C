/*
 * A063144  Dimension of the space of weight 2n cusp forms for Gamma_0( 76 ).
 * https://oeis.org/A063144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8, a(n) = 20*n - 13 for n > 1
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This also satisfies a(n) = 2*a(n-1) - a(n-2) for n > 3 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)8;
    } else {
        /* compute 20*n - 13 using seq_t arithmetic */
        return (seq_t)n * (seq_t)20 - (seq_t)13;
    }
}
