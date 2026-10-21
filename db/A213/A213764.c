/*
 * A213764  Antidiagonal sums of the convolution array A213762.
 * https://oeis.org/A213764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -10 + 5*2^(n+1) - 7*n - 2*n^2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n + 1; ++i) {
        pow2 = pow2 * (seq_t)2;
    }

    seq_t n_seq = (seq_t)n;
    seq_t term1 = pow2 * (seq_t)5;     /* 5*2^(n+1) */
    seq_t term2 = (seq_t)7 * n_seq;     /* 7*n   */
    seq_t n_sq  = n_seq * n_seq;
    seq_t term3 = (seq_t)2 * n_sq;      /* 2*n^2 */

    seq_t result = term1 - term2 - term3 - (seq_t)10;
    return result;
}
