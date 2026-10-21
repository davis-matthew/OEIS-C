/*
 * A063162  Dimension of the space of weight 2n cusp forms for Gamma_0( 94 ).
 * https://oeis.org/A063162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n - 14 for n > 1, a(1) = 11
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n == 0) {
        return (seq_t)0;          /* undefined case – return 0 safely */
    }
    if (n == 1) {
        return (seq_t)11;
    }
    seq_t nn = (seq_t)n;
    return ((seq_t)24 * nn) - (seq_t)14;
}
