/*
 * A071233  a(n) = 2*(n-1)*(n^2 + 1).
 * https://oeis.org/A071233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*(n*n + 1)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1290, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t once to avoid accidental idx_t arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute (n-1) */
    seq_t n_minus1 = n_seq - 1;

    /* Compute n^2 + 1 */
    seq_t n_sq = n_seq * n_seq;
    seq_t n_sq_plus1 = n_sq + 1;

    /* Compute the product and double it */
    seq_t prod = n_minus1 * n_sq_plus1;
    return prod * 2;
}
