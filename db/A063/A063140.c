/*
 * A063140  Dimension of the space of weight 2n cusp forms for Gamma_0( 72 ).
 * https://oeis.org/A063140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A063140:
   a(1) = 5
   a(n) = 24*n - 20          for n > 1
   (equivalently, a(n) = 2*a(n-1) - a(n-2) for n > 3) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;     /* undefined for n < 1, return 0 */
    }
    if (n == 1) {
        return (seq_t)5;
    }
    seq_t tmp = (seq_t)24 * (seq_t)n;
    return tmp - (seq_t)20;
}
