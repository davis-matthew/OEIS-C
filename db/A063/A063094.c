/*
 * A063094  Dimension of the space of weight 2n cusp forms for Gamma_0( 26 ).
 * https://oeis.org/A063094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*n − 6 if n is odd; a(n) = 7*n − 5 if n is even. * a(1)
 *            = 2. This implementation uses O(1) arithmetic and works for any
 *            * values of n ≥ 1 that fit into idx_t
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset base case */
    if (n == (idx_t)1)
        return (seq_t)2;

    /* Convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute 7*n */
    seq_t term = (seq_t)7 * n_seq;

    /* Subtract 6 for odd n, 5 for even n */
    if ((n & (idx_t)1) == (idx_t)0)      /* even */
        term -= (seq_t)5;
    else                                 /* odd */
        term -= (seq_t)6;

    return term;
}
