/*
 * A081268  Diagonal of triangular spiral in A051682.
 * https://oeis.org/A081268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n^2 + 13*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to the sequence type first */
    seq_t n_seq = (seq_t)n;
    /* compute the quadratic expression in seq_t arithmetic */
    seq_t term1 = (seq_t)9 * n_seq * n_seq;
    seq_t term2 = (seq_t)13 * n_seq;
    seq_t sum   = term1 + term2 + (seq_t)2;
    return sum / (seq_t)2;
}
