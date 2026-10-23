/*
 * A193656  Q-residue of the triangle p(n,k)=(2^(n - k))*5^k, 0<=k<=n, where Q
 *          is the triangular array (t(i,j)) given by t(i,j)=1. (See
 *          Comments.)
 * https://oeis.org/A193656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*5^n-2*4^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute 5^n and 4^n by repeated multiplication. */
    seq_t pow5 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
        pow4 *= (seq_t)4;
    }
    /* a(n) = 3*5^n - 2*4^n */
    seq_t term1 = (seq_t)3 * pow5;
    seq_t term2 = (seq_t)2 * pow4;
    return term1 - term2;
}
