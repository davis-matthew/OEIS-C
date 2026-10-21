/*
 * A063224  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          60 ).
 * https://oeis.org/A063224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor(2*n/3)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* convert index to sequence type */
    seq_t t  = (nn * 2) / 3;      /* floor(2*n/3) */
    return t * 2;                 /* multiply by 2 to get a(n) */
}
